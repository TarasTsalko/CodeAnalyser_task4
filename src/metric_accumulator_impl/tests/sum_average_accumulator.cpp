#include "metric_accumulator_impl/sum_average_accumulator.hpp"
#include "analyse.hpp"
#include "utils.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(SumAvarigeAccammulatorMetricTest, SumAvarigeAccammulatorMetricTest) {

    std::vector<std::string> inFiles = {"dummy_class.py", "data_processor.py"};

    analyser::metric::MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::CodeLinesCountMetric>());
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::CountParametersMetric>());
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::CyclomaticComplexityMetric>());

    auto analyse = analyser::AnalyseFunctions(inFiles, metric_extractor);
    SumAverageAccumulator acc;
    CalculateAccamullatedValue(analyse, acc, "CodeLinesCountMetric");
    EXPECT_EQ(acc.Get().average, 3);
    EXPECT_EQ(acc.Get().sum, 38);
    acc.Reset();

    CalculateAccamullatedValue(analyse, acc, "CyclomaticComplexityMetric");
    EXPECT_EQ(acc.Get().average, 1);
    EXPECT_EQ(acc.Get().sum, 20);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
