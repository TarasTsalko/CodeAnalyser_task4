#include "metric_accumulator_impl/average_accumulator.hpp"
#include "analyse.hpp"
#include "utils.hpp"

#include <vector>

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

// TODO: Спросить совета у ревьювера (стоит ли пытаться обобщить)
// TODO:
/*
    AvarigeAccammulatorMetricSplittedByFilesCCTest
    AvarigeAccammulatorMetricSplittedByFilesLCTest
    SumAvarigeAccammulatorMetricSplittedByFilesLCTest
    SumAvarigeAccammulatorMetricSplittedByFilesCCTest
    код в данных тестах можно попробовать обобщить,
    но пока оставил так,
    так ка вынести можно тллько:

        analyser::metric::MetricExtractor metric_extractor;
        metric_extractor.RegisterMetric(std::make_unique<metric_impl::CodeLinesCountMetric>());
        metric_extractor.RegisterMetric(std::make_unique<metric_impl::CountParametersMetric>());
        metric_extractor.RegisterMetric(std::make_unique<metric_impl::CyclomaticComplexityMetric>());
        auto analyse = analyser::AnalyseFunctions(inFiles, metric_extractor);

        при этом передав параметр inFiles, сделав функцию щаблонной для задания CountParametersMetric,
        CountParametersMetric, CyclomaticComplexityMetric. Так что пока KISS(Keep It Short and Simple).
*/

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

// CC - CyclomaticComplexityMetric
TEST(AvarigeAccammulatorMetricSplittedByFilesCCTest, AvarigeAccammulatorMetricSplittedByFilesCCTest) {
    std::vector<std::string> inFiles = {"dummy_class.py", "data_processor.py"};

    analyser::metric::MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::CyclomaticComplexityMetric>());
    auto analyse = analyser::AnalyseFunctions(inFiles, metric_extractor);
    const auto splittedByFiles = analyser::SplitByFiles(analyse);
    ASSERT_EQ(splittedByFiles.size(), 2u);

    AverageAccumulator acc;
    CalculateAccamullatedValue(splittedByFiles[0], acc, "CyclomaticComplexityMetric");
    auto result = acc.Get();
    EXPECT_EQ(result, 1);
    acc.Reset();

    CalculateAccamullatedValue(splittedByFiles[1], acc, "CyclomaticComplexityMetric");

    // функций в файле 6, сложность 15, целое значение выходит 2
    // Вопрос к ревьюверу не имеет ли смысл считать дробные величины?
    // Хотя для сложности  это будет выглядить странно.
    result = acc.Get();
    EXPECT_EQ(result, 2);
}

// LC - LineCount
TEST(AvarigeAccammulatorMetricSplittedByFilesLCTest, AvarigeAccammulatorMetricSplittedByFilesLCTest) {
    std::vector<std::string> inFiles = {"dummy_class.py", "data_processor.py"};

    analyser::metric::MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::CodeLinesCountMetric>());
    auto analyse = analyser::AnalyseFunctions(inFiles, metric_extractor);
    const auto splittedByFiles = analyser::SplitByFiles(analyse);
    ASSERT_EQ(splittedByFiles.size(), 2u);

    AverageAccumulator acc;
    CalculateAccamullatedValue(splittedByFiles[0], acc, "CodeLinesCountMetric");
    int result = acc.Get();
    // 5 методов, 5 строк
    EXPECT_EQ(result, 1);
    acc.Reset();

    CalculateAccamullatedValue(splittedByFiles[1], acc, "CodeLinesCountMetric");

    // 33 строки, 6 методов
    result = acc.Get();
    EXPECT_EQ(result, 5);
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

TEST(AvarigeAccammulatorDivByZeroTest, AvarigeAccammulatorDivByZeroTest) {
    AverageAccumulator acc;
    EXPECT_THROW(
        {
            try {
                acc.Finalize();
                acc.Get();
            } catch (const std::runtime_error &e) {
                EXPECT_STREQ("AverageAccumulator::Finalize: деление"
                             " на ноль при подсчете усредненного значения метрики",
                             e.what());
                throw;
            }
        },
        std::exception);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
