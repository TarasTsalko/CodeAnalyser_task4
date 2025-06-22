#pragma once
#include <string>
#include <unistd.h>

#include "metric.hpp"

namespace analyser::metric::metric_impl {

struct CyclomaticComplexityMetric : IMetric {
    virtual ~CyclomaticComplexityMetric() = default;

protected:
    virtual MetricResult::ValueType CalculateImpl(const function::Function &f) const;
    virtual std::string Name() const;
};

}  // namespace analyser::metric::metric_impl
