#include "metric_accumulator_impl/average_accumulator.hpp"

#include <unistd.h>

#include <algorithm>
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

namespace analyser::metric_accumulator::metric_accumulator_impl {

void AverageAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    sum += metric_result.value;
    count++;
}

void AverageAccumulator::Finalize() {
    // добавить exception при делении на 0.0
    average = sum / count;
    is_finalized = true;
}

void AverageAccumulator::Reset() {
    sum = 0;
    count = 0;
    average = 0;
    is_finalized = false;
}

double AverageAccumulator::Get() const {
    assert(is_finalized);  // возможно потом стоит бросить исключение
    return average;
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
