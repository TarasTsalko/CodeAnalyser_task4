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

namespace rv = std::ranges::views;
namespace rs = std::ranges;

auto AnalyseFunctions(const std::vector<std::string> &files,
                      const analyser::metric::MetricExtractor &metric_extractor) {

    // TODO: первоночальный вариант с циклами, что бы отладить логику,
    // а потом переписать с использованием ranges и views
    using namespace analyser::metric;
    MetricResults results;
    using namespace analyser::function;
    FunctionExtractor functionExtractor;
    auto all_metrics = files | std::views::transform([&](const auto &file) { return functionExtractor.Get(file); }) |
                       std::views::join |
                       std::views::transform([&](const auto &func) { return metric_extractor.Get(func); }) |
                       std::views::join;
    results.reserve(std::ranges::distance(all_metrics));
    std::ranges::copy(all_metrics, std::back_inserter(results));
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

}  // namespace analyser
