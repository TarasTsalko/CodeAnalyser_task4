#include "metric_accumulator.hpp"

#include <unistd.h>

#include <algorithm>
#include <any>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyser::metric_accumulator {

void MetricsAccumulator::AccumulateNextFunctionResults(const std::vector<metric::MetricResult> &metric_results) const {
    std::ranges::for_each(metric_results, [this](const auto &result) {
        const auto it = this->accumulators.find(result.metric_name);
        assert(it != accumulators.end());
        it->second->Accumulate(result);
    });
}

void MetricsAccumulator::ResetAccumulators() {
    std::ranges::for_each(accumulators | std::views::values, [](const auto &obj) {
        if (obj)
            obj->Reset();
    });
}

}  // namespace analyser::metric_accumulator
