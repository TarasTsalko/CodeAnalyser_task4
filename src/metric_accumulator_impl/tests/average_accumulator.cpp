#include "metric_accumulator_impl/average_accumulator.hpp"
#include "analyse.hpp"
#include "utils.hpp"

#include <vector>

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(AvarigeAccammulatorMetricTest, AvarigeAccammulatorMetricTest) {

    std::vector<std::string> inFiles = {"dummy_class.py", "data_processor.py"};

    analyser::metric::MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::CodeLinesCountMetric>());
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::CountParametersMetric>());
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::CyclomaticComplexityMetric>());

    auto analyse = analyser::AnalyseFunctions(inFiles, metric_extractor);
    AverageAccumulator acc;
    CalculateAccamullatedValue(analyse, acc, "CodeLinesCountMetric");
    EXPECT_EQ(acc.Get(), 3);
    acc.Reset();

    CalculateAccamullatedValue(analyse, acc, "CountParametersMetric");
    EXPECT_EQ(acc.Get(), 1);
    acc.Reset();

    CalculateAccamullatedValue(analyse, acc, "CyclomaticComplexityMetric");
    // сумарнвя цикломатическа сложность в двух файлах 20, количество методов 11, среднее значение 1
    EXPECT_EQ(acc.Get(), 1);
}

TEST(AvarigeAccammulatorFinilizeTest, AvarigeAccammulatorFinilizeTest) {
    std::vector<std::string> inFiles = {"dummy_class.py", "data_processor.py"};

    analyser::metric::MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::CodeLinesCountMetric>());
    auto analyse = analyser::AnalyseFunctions(inFiles, metric_extractor);
    AverageAccumulator acc;
    EXPECT_THROW(
        {
            try {
                CalculateAccamullatedValue(analyse, acc, "CodeLinesCountMetric", false);
                acc.Get();
            } catch (const std::runtime_error &e) {
                EXPECT_STREQ("AverageAccumulator::Get: процесс аккомуляции метрик не был финализирован", e.what());
                throw;
            }
        },
        std::exception);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
