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

// CC - CyclomaticComplexityMetric
TEST(SumAvarigeAccammulatorMetricSplittedByFilesCCTest, SumAvarigeAccammulatorMetricSplittedByFilesCCTest) {
    std::vector<std::string> inFiles = {"dummy_class.py", "data_processor.py"};

    analyser::metric::MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::CyclomaticComplexityMetric>());
    auto analyse = analyser::AnalyseFunctions(inFiles, metric_extractor);
    const auto splittedByFiles = analyser::SplitByFiles(analyse);
    ASSERT_EQ(splittedByFiles.size(), 2u);

    SumAverageAccumulator acc;
    CalculateAccamullatedValue(splittedByFiles[0], acc, "CyclomaticComplexityMetric");
    auto result = acc.Get();
    EXPECT_EQ(result.sum, 5);
    EXPECT_EQ(result.average, 1);
    acc.Reset();

    CalculateAccamullatedValue(splittedByFiles[1], acc, "CyclomaticComplexityMetric");

    // функций в файле 6, сложность 15, целое значение выходит 2
    // Вопрос к ревьюверу не имеет ли смысл считать дробные величины?
    // Хотя для сложности  это будет выглядить странно.
    result = acc.Get();
    EXPECT_EQ(result.sum, 15);
    EXPECT_EQ(result.average, 2);
}

TEST(SumAvarigeAccammulatorMetricSplittedByFilesLCTest, SumAvarigeAccammulatorMetricSplittedByFilesLCTest) {
    std::vector<std::string> inFiles = {"dummy_class.py", "data_processor.py"};

    analyser::metric::MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::CodeLinesCountMetric>());
    auto analyse = analyser::AnalyseFunctions(inFiles, metric_extractor);
    const auto splittedByFiles = analyser::SplitByFiles(analyse);
    ASSERT_EQ(splittedByFiles.size(), 2u);

    SumAverageAccumulator acc;
    CalculateAccamullatedValue(splittedByFiles[0], acc, "CodeLinesCountMetric");
    auto result = acc.Get();
    EXPECT_EQ(result.sum, 5);
    EXPECT_EQ(result.average, 1);
    acc.Reset();

    CalculateAccamullatedValue(splittedByFiles[1], acc, "CodeLinesCountMetric");

    result = acc.Get();
    EXPECT_EQ(result.sum, 33);
    EXPECT_EQ(result.average, 5);
}

TEST(SumAvarigeAccammulatorFinilizeTest, SumAvarigeAccammulatorFinilizeTest) {
    std::vector<std::string> inFiles = {"dummy_class.py", "data_processor.py"};

    analyser::metric::MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::CodeLinesCountMetric>());
    auto analyse = analyser::AnalyseFunctions(inFiles, metric_extractor);
    SumAverageAccumulator acc;
    EXPECT_THROW(
        {
            try {
                CalculateAccamullatedValue(analyse, acc, "CodeLinesCountMetric", false);
                acc.Get();
            } catch (const std::runtime_error &e) {
                EXPECT_STREQ("SumAverageAccumulator::Get: процесс аккомуляции метрик не был финализирован", e.what());
                throw;
            }
        },
        std::exception);
}

TEST(SumAvarigeAccammulatorDivByZeroTest, SumAvarigeAccammulatorDivByZeroTest) {
    SumAverageAccumulator acc;
    EXPECT_THROW(
        {
            try {
                acc.Finalize();
                acc.Get();
            } catch (const std::runtime_error &e) {
                EXPECT_STREQ("SumAverageAccumulator::Finalize: деление"
                             " на ноль при подсчете усредненного значения метрики",
                             e.what());
                throw;
            }
        },
        std::exception);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
