#include "metric_accumulator_impl/categorical_accumulator.hpp"

#include <unistd.h>

namespace analyser::metric_accumulator::metric_accumulator_impl {

void CategoricalAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    categories_freq[std::get<std::string>(metric_result.value)]++;
}

void CategoricalAccumulator::Finalize() { is_finalized = true; }

void CategoricalAccumulator::Reset() {
    categories_freq.clear();
    is_finalized = false;
}

const std::unordered_map<std::string, int> &CategoricalAccumulator::Get() const {
    if (!is_finalized)
        throw std::runtime_error("CategoricalAccumulator::Get: процесс аккомуляции метрик не был финализирован");
    return categories_freq;
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
