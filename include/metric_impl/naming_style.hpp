#pragma once
#include <unistd.h>

#include "metric.hpp"

namespace analyser::metric::metric_impl {

struct NamingStyleMetric final : public IMetric {
    virtual ~NamingStyleMetric() = default;

protected:
    virtual MetricResult::ValueType CalculateImpl(const function::Function &f) const;
    virtual std::string Name() const;
};

}  // namespace analyser::metric::metric_impl
