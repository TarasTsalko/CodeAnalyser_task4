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
#include <sstream>
#include <string>
#include <variant>
#include <vector>

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

auto SplitByClasses(const auto &analysis) {
    // здесь ваш код
}

auto SplitByFiles(const auto &analysis) {
    // здесь ваш код
}

void AccumulateFunctionAnalysis(const auto &analysis,
                                const analyser::metric_accumulator::MetricsAccumulator &accumulator) {
    // здесь ваш код
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

}  // namespace analyser
