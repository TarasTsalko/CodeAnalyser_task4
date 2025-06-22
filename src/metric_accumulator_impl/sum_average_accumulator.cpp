#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <unistd.h>
#include <variant>

namespace analyser::metric_accumulator::metric_accumulator_impl {

void SumAverageAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    sum += std::get<int>(metric_result.value);
    count++;
}

void SumAverageAccumulator::Finalize() {
    if (count == 0)
        throw std::runtime_error("SumAverageAccumulator::Finalize: деление"
                                 " на ноль при подсчете усредненного значения метрики");
    average = sum / count;
    is_finalized = true;
}

void SumAverageAccumulator::Reset() {
    sum = 0;
    count = 0;
    average = 0;
    is_finalized = false;
}

SumAverageAccumulator::SumAverage SumAverageAccumulator::Get() const {
    if (!is_finalized)
        throw std::runtime_error("SumAverageAccumulator::Get: процесс аккомуляции метрик не был финализирован");
    return SumAverageAccumulator::SumAverage{sum, average};
}

// здесь ваш код

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
