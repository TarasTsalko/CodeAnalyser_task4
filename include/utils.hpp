#pragma once

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"

#include <charconv>
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

template <typename MetricType>
bool CheckMetric(const std::string &fileName, std::string_view metric_name, int val) {
    using namespace analyser;
    auto functions = CreateFunctions(fileName);
    std::unique_ptr<metric::IMetric> metric = std::make_unique<MetricType>();
    return std::ranges::all_of(functions, [&metric, metric_name, val](const auto &f) {
        const auto metricRes = metric->Calculate(f);
        return metric_name == metricRes.metric_name && metricRes.value == val;
    });
}
