#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <print>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "./metric_accumulator_impl/accumulators.hpp"
#include "./metric_impl/metrics.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"

namespace analyser {

using namespace analyser::metric;
using namespace analyser::file;
using namespace analyser::function;

namespace vs = std::views;
namespace rv = std::ranges::views;
namespace rs = std::ranges;

using MetricsToFuncs = std::vector<std::pair<Function, MetricResults>>;

auto AnalyseFunctions(const std::vector<std::string> &files,
                      const analyser::metric::MetricExtractor &metric_extractor) {

    using Functions = std::vector<Function>;
    MetricsToFuncs results;

    Functions functions;
    FunctionExtractor functionExtractor;

    auto all_functions = files | vs::transform([&functionExtractor](const auto &fullfileName) {
                             const File file(fullfileName);
                             return functionExtractor.Get(file);  // returns std::vector<FunctionType>
                         }) |
                         vs::join;

    results.reserve(rs::distance(all_functions));
    rs::transform(all_functions, std::back_inserter(results),
                  [&metric_extractor](const auto &func) { return std::pair{func, metric_extractor.Get(func)}; });
    return results;
}

auto SplitByClasses(const MetricsToFuncs &analysis) {

    auto grouped = analysis | std::views::filter([](const auto &item) { return item.first.class_name.has_value(); }) |
                   std::views::transform([](auto &&item) { return std::move(item); }) |
                   std::views::chunk_by([](const auto &a, const auto &b) {
                       return a.first.class_name.value() == b.first.class_name.value();
                   });

    // Вопрос к reviewer-у
    // возвращаемы тип данных это вектор MetricsToFuncs,
    // в свою очередь MetricsToFuncs, который std::vector>;,
    // MetricResults --> std::vector<...>, можно ли сделать более оптимально (так как
    // из-за вложенных векторов будет просадка производительности) // или так  и задумано?
    auto groups = grouped | std::views::transform([](auto &&subrange) {
                      MetricsToFuncs group;
                      std::ranges::move(subrange, std::back_inserter(group));
                      return group;
                  }) |
                  std::ranges::to<std::vector>();

    return groups;
}

auto SplitByFiles(const auto &analysis) {

    auto groups = analysis |
                  vs::chunk_by([](const auto &a, const auto &b) { return a.first.filename == b.first.filename; }) |
                  vs::transform([](auto &&subrange) {
                      MetricsToFuncs group;
                      std::ranges::move(subrange, std::back_inserter(group));
                      return group;
                  }) |
                  std::ranges::to<std::vector>();

    return groups;
}

void AccumulateFunctionAnalysis(const auto &analysis,
                                const analyser::metric_accumulator::MetricsAccumulator &accumulator) {
    std::ranges::for_each(
        analysis, [&accumulator](const auto &group) { accumulator.AccumulateNextFunctionResults(group.second); });
}

void PrintResultAnalyseFunction(const MetricsToFuncs &metricsToFuncs) {
    auto print_info = [](const auto &info) {
        const auto &func = info.first;
        if (func.class_name)
            std::print("{}[::{}]::{}\n", func.filename, *func.class_name, func.name);
        else
            std::print("{}[::None]::{}\n", func.filename, func.name);

        // Inner iteration over metrics using ranges
        std::ranges::for_each(info.second,
                              [](const auto &metric) { std::print("\t{}: {}\n", metric.metric_name, metric.value); });
    };
    std::ranges::for_each(metricsToFuncs, print_info);
}

void PrintSummaryResults(const auto &analysis, const analyser::metric_accumulator::MetricsAccumulator &accumulator) {
    namespace metric_impl = metric::metric_impl;
    namespace accamulator_impl = metric_accumulator::metric_accumulator_impl;
    namespace accumulator_interface = analyser::metric_accumulator;

    auto uniqueMetrics =
        analysis | std::views::transform([](const auto &info) {
            return info.second | std::views::transform([](const auto &metric) { return metric.metric_name; });
        }) |
        std::views::join | std::ranges::to<std::set<std::string>>();

    // Обрабатываем каждую уникальную метрику
    std::ranges::for_each(uniqueMetrics, [&accumulator](const std::string &metricName) {
        const auto &metricAcc = accumulator.GetFinalizedAccumulator<accumulator_interface::IAccumulator>(metricName);

        if (const auto *acc = dynamic_cast<const accamulator_impl::SumAverageAccumulator *>(&metricAcc)) {
            const auto sumAndAverageRes = acc->Get();
            std::cout << std::format("\t {} Sum: {}\n", metricName, sumAndAverageRes.sum);
            std::cout << std::format("\t {} Average: {}\n", metricName, sumAndAverageRes.average);
        } else if (const auto *acc = dynamic_cast<const accamulator_impl::AverageAccumulator *>(&metricAcc)) {
            std::cout << std::format("\t {} Average: {}\n", metricName, acc->Get());
        }
    });
}

void PrintResultAnalyseSplittedByGroup(const auto &analysis,
                                       analyser::metric_accumulator::MetricsAccumulator &accumulator,
                                       std::string_view mode) {
    namespace metric_impl = metric::metric_impl;
    namespace accamulator_impl = metric_accumulator::metric_accumulator_impl;
    namespace accumulator_interface = analyser::metric_accumulator;

    std::ranges::for_each(analysis, [&accumulator, mode](const auto &group) {
        analyser::AccumulateFunctionAnalysis(group, accumulator);

        std::string field;
        if (mode == "file")
            std::print("Accumulated Analysis for file: {}\n", group.begin()->first.filename);
        else if (mode == "class")
            std::print("Accumulated Analysis for class: {}\n", *group.begin()->first.class_name);

        const auto &data = group.begin()->second;
        std::ranges::for_each(data, [&accumulator](const auto &metric) {
            const auto &metricAcc =
                accumulator.GetFinalizedAccumulator<accumulator_interface::IAccumulator>(metric.metric_name);

            if (const auto *acc = dynamic_cast<const accamulator_impl::SumAverageAccumulator *>(&metricAcc)) {
                const auto sumAndAverageRes = acc->Get();
                std::print("\t {} Sum: {}\n", metric.metric_name, sumAndAverageRes.sum);
                std::print("\t {} Average: {}\n", metric.metric_name, sumAndAverageRes.average);
            } else if (const auto *acc = dynamic_cast<const accamulator_impl::AverageAccumulator *>(&metricAcc)) {
                std::print("\t {} Average: {}\n", metric.metric_name, acc->Get());
            }
        });

        accumulator.ResetAccumulators();
    });
}

}  // namespace analyser
