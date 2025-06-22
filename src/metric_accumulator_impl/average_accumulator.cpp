#include "metric_accumulator_impl/average_accumulator.hpp"

#include <unistd.h>
#include <variant>

namespace analyser::metric_accumulator::metric_accumulator_impl {

void AverageAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    sum += std::get<int>(metric_result.value);
    count++;
}

void AverageAccumulator::Finalize() {
    if (count == 0)
        throw std::runtime_error("AverageAccumulator::Finalize: деление"
                                 " на ноль при подсчете усредненного значения метрики");
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
    if (!is_finalized)
        throw std::runtime_error("AverageAccumulator::Get: процесс аккомуляции метрик не был финализирован");
    return average;
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
