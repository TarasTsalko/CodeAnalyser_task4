#include "metric_impl/parameters_count.hpp"

#include "utils.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

// PC - paameters count
TEST(PC_SingleLineCommentTest, PC_SingleLineCommentTest) {
    const std::string fileName = "comments.py";
    EXPECT_TRUE(CheckMetric<CountParametersMetric>(fileName, "CountParametersMetric", 3));
}

TEST(PC_MultiLinesCommentTest, PC_MultiLinesCommentTest) {
    const std::string fileName = "multi_lines_comments.py";
    EXPECT_TRUE(CheckMetric<CountParametersMetric>(fileName, "CountParametersMetric", 3));
}

TEST(PC_ExceptionConstructionTest, PC_ExceptionConstructionTest) {
    const std::string fileName = "exceptions.py";
    EXPECT_TRUE(CheckMetric<CountParametersMetric>(fileName, "CountParametersMetric", 0));
}

TEST(PC_IfConstructionTest, PC_IfConstructionTest) {
    const std::string fileName = "if.py";
    EXPECT_TRUE(CheckMetric<CountParametersMetric>(fileName, "CountParametersMetric", 1));
}

TEST(PC_LoopsConstructionTest, PC_LoopsConstructionTest) {
    const std::string fileName = "loops.py";
    EXPECT_TRUE(CheckMetric<CountParametersMetric>(fileName, "CountParametersMetric", 1));
}

TEST(PC_ManyLinesTest, PC_ManyLinesTest) {
    const std::string fileName = "many_lines.py";
    EXPECT_TRUE(CheckMetric<CountParametersMetric>(fileName, "CountParametersMetric", 0));
}

TEST(PC_ManyParametersTest, PC_ManyParametersTest) {
    const std::string fileName = "many_parameters.py";
    EXPECT_TRUE(CheckMetric<CountParametersMetric>(fileName, "CountParametersMetric", 5));
}

TEST(PC_MatchCaseTest, PC_MatchCaseTest) {
    const std::string fileName = "match_case.py";
    EXPECT_TRUE(CheckMetric<CountParametersMetric>(fileName, "CountParametersMetric", 1));
}

TEST(PC_NestedIfTest, PC_NestedIfTest) {
    const std::string fileName = "nested_if.py";
    EXPECT_TRUE(CheckMetric<CountParametersMetric>(fileName, "CountParametersMetric", 2));
}

TEST(PC_SimpleTest, PC_SimpleTest) {
    const std::string fileName = "simple.py";
    EXPECT_TRUE(CheckMetric<CountParametersMetric>(fileName, "CountParametersMetric", 0));
}

TEST(PC_TernaryTest, PC_TernaryTest) {
    const std::string fileName = "ternary.py";
    EXPECT_TRUE(CheckMetric<CountParametersMetric>(fileName, "CountParametersMetric", 1));
}

// здесь ваш код

}  // namespace analyser::metric::metric_impl
