#include "metric_accumulator_impl/categorical_accumulator.hpp"

#include "analyse.hpp"
#include "utils.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(CategoricalAccammulatorMetricTest, CategoricalAccammulatorMetricTest) {

    std::vector<std::string> inFiles = {"dummy_class.py", "data_processor.py"};

    analyser::metric::MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::NamingStyleMetric>());

    auto analyse = analyser::AnalyseFunctions(inFiles, metric_extractor);
    CategoricalAccumulator acc;
    CalculateAccamullatedValue(analyse, acc, "NamingStyleMetric");

    const auto &result = acc.Get();
    // в этих двух файлах 11 функций(методов) из 3 котигорий: snake_case, lower_case, unknown
    ASSERT_EQ(result.size(), 3u);

    auto it = result.find("snake case");
    ASSERT_TRUE(it != result.end());
    ASSERT_EQ(it->second, 8u);

    it = result.find("lowercase");
    ASSERT_TRUE(it != result.end());
    ASSERT_EQ(it->second, 1u);

    it = result.find("unknown");
    ASSERT_TRUE(it != result.end());
    ASSERT_EQ(it->second, 2u);
}

TEST(CategoricalAccammulatorFinilizeTest, CategoricalAccammulatorFinilizeTest) {
    std::vector<std::string> inFiles = {"dummy_class.py", "data_processor.py"};

    analyser::metric::MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::NamingStyleMetric>());
    auto analyse = analyser::AnalyseFunctions(inFiles, metric_extractor);
    CategoricalAccumulator acc;
    EXPECT_THROW(
        {
            try {
                CalculateAccamullatedValue(analyse, acc, "NamingStyleMetric", false);
                acc.Get();
            } catch (const std::runtime_error &e) {
                EXPECT_STREQ("CategoricalAccumulator::Get: процесс аккомуляции метрик не был финализирован", e.what());
                throw;
            }
        },
        std::exception);
}

TEST(CategoricalMetricSplittedByFilesNSTest, CategoricalMetricSplittedByFilesNSTest) {
    std::vector<std::string> inFiles = {"dummy_class.py", "data_processor.py"};

    analyser::metric::MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::NamingStyleMetric>());
    auto analyse = analyser::AnalyseFunctions(inFiles, metric_extractor);
    const auto splittedByFiles = analyser::SplitByFiles(analyse);
    ASSERT_EQ(splittedByFiles.size(), 2u);

    CategoricalAccumulator acc;
    {
        CalculateAccamullatedValue(splittedByFiles[0], acc, "NamingStyleMetric");
        const auto &result = acc.Get();
        ASSERT_EQ(result.size(), 3u);

        auto it = result.find("snake case");
        ASSERT_TRUE(it != result.end());
        ASSERT_EQ(it->second, 3u);

        it = result.find("lowercase");
        ASSERT_TRUE(it != result.end());
        ASSERT_EQ(it->second, 1u);

        it = result.find("unknown");
        ASSERT_TRUE(it != result.end());
        ASSERT_EQ(it->second, 1u);
    }

    acc.Reset();
    {
        CalculateAccamullatedValue(splittedByFiles[1], acc, "NamingStyleMetric");
        const auto &result = acc.Get();
        ASSERT_EQ(result.size(), 2u);

        auto it = result.find("snake case");
        ASSERT_TRUE(it != result.end());
        ASSERT_EQ(it->second, 5u);

        it = result.find("unknown");
        ASSERT_TRUE(it != result.end());
        ASSERT_EQ(it->second, 1u);
    }
}

TEST(CategoricalMetricSplittedBylassesNSTest, CategoricalMetricSplittedBylassesNSTest) {
    std::vector<std::string> inFiles = {"dummy_class.py", "data_processor.py"};

    analyser::metric::MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::NamingStyleMetric>());
    auto analyse = analyser::AnalyseFunctions(inFiles, metric_extractor);
    const auto splittedByClasses = analyser::SplitByClasses(analyse);
    ASSERT_EQ(splittedByClasses.size(), 2u);

    CategoricalAccumulator acc;
    {
        CalculateAccamullatedValue(splittedByClasses[0], acc, "NamingStyleMetric");
        const auto &result = acc.Get();
        ASSERT_EQ(result.size(), 3u);

        auto it = result.find("snake case");
        ASSERT_TRUE(it != result.end());
        ASSERT_EQ(it->second, 3u);

        it = result.find("lowercase");
        ASSERT_TRUE(it != result.end());
        ASSERT_EQ(it->second, 1u);

        it = result.find("unknown");
        ASSERT_TRUE(it != result.end());
        ASSERT_EQ(it->second, 1u);
    }

    acc.Reset();
    {
        CalculateAccamullatedValue(splittedByClasses[1], acc, "NamingStyleMetric");
        const auto &result = acc.Get();
        ASSERT_EQ(result.size(), 2u);

        auto it = result.find("snake case");
        ASSERT_TRUE(it != result.end());
        ASSERT_EQ(it->second, 5u);

        it = result.find("unknown");
        ASSERT_TRUE(it != result.end());
        ASSERT_EQ(it->second, 1u);
    }
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
