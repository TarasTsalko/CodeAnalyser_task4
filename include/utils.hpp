#pragma once

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"

#include <algorithm>
#include <charconv>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>

inline int ToInt(std::string_view value) {
    int result{};
    auto [parse_end_ptr, error_code] = std::from_chars(value.begin(), value.end(), result);
    if (error_code != std::errc{} || parse_end_ptr != value.data() + value.size()) {
        throw std::invalid_argument("Cannot convert '" + std::string(value) + "' to integral");
    }
    return result;
}

inline void RemoveSpaces(std::string &str) {
    const auto [first, last] = std::ranges::remove_if(str, [](unsigned char c) { return std::isspace(c); });
    str.erase(first, last);
}

// Для тестирования метрики
inline auto CreateFunctions(const std::string &fileName) {
    using namespace analyser;
    const file::File sourceFile(fileName);
    function::FunctionExtractor extructor;
    auto functions = extructor.Get(sourceFile);
    return functions;
}

template <typename MetricType, typename ValueType>
bool CheckMetric(const std::string &fileName, std::string_view metric_name, ValueType &&val) {
    using namespace analyser;
    auto functions = CreateFunctions(fileName);
    std::unique_ptr<metric::IMetric> metric = std::make_unique<MetricType>();
    return std::ranges::all_of(functions, [&metric, metric_name, val](const auto &f) {
        const auto metricRes = metric->Calculate(f);
        return metric_name == metricRes.metric_name && std::get<ValueType>(metricRes.value) == val;
    });
}

inline auto CalculateAccamullatedValue(const auto &analyse, auto &acc, std::string_view metric_name,
                                       bool make_finilize = true) {
    auto metrics_view = analyse | std::views::values | std::views::join;
    std::ranges::for_each(metrics_view, [&acc, metric_name](const auto &metric) {
        if (metric.metric_name == metric_name) {
            acc.Accumulate(metric);
        }
    });
    if (make_finilize)
        acc.Finalize();
}
