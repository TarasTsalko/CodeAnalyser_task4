#pragma once
#include <unistd.h>

#include <algorithm>
#include <any>
#include <array>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "metric.hpp"

namespace rv = std::ranges::views;
namespace rs = std::ranges;

namespace analyser::metric_accumulator {

struct IAccumulator {
    virtual void Accumulate(const metric::MetricResult &metric_result) = 0;
    virtual void Finalize() = 0;
    virtual void Reset() = 0;
    virtual ~IAccumulator() = default;

protected:
    bool is_finalized = false;
};

struct MetricsAccumulator {
    template <typename Accumulator>
    void RegisterAccumulator(const std::string &metric_name, std::unique_ptr<Accumulator> acc) {
        accumulators.try_emplace(metric_name, std::move(acc));
    }
    template <typename Accumulator>
    const Accumulator &GetFinalizedAccumulator(const std::string &metric_name) const {
        auto it = accumulators.find(metric_name);
        if (it == accumulators.end())
            std::runtime_error(std::format("В MetricAccumulator не найдена метрика: {}", metric_name));
        it->second->Finalize();
        return *it->second.get();
    }
    void AccumulateNextFunctionResults(const std::vector<metric::MetricResult> &metric_results) const;

    void ResetAccumulators();

private:
    std::unordered_map<std::string, std::shared_ptr<IAccumulator>> accumulators;
};

}  // namespace analyser::metric_accumulator
