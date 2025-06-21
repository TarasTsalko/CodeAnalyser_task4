#include "metric_impl/cyclomatic_complexity.hpp"

#include "utils.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

// CC - cyclomatic complexity
TEST(CC_SingleLineCommentTest, CC_SingleLineCommentTest) {
    const std::string fileName = "comments.py";
    EXPECT_TRUE(CheckMetric<CyclomaticComplexityMetric>(fileName, "CyclomaticComplexityMetric", 1));
}

TEST(CC_MultiLinesCommentTest, CC_MultiLinesCommentTest) {
    const std::string fileName = "multi_lines_comments.py";
    EXPECT_TRUE(CheckMetric<CyclomaticComplexityMetric>(fileName, "CyclomaticComplexityMetric", 1));
}

TEST(CC_ExceptionConstructionTest, CC_ExceptionConstructionTest) {
    const std::string fileName = "exceptions.py";
    EXPECT_TRUE(CheckMetric<CyclomaticComplexityMetric>(fileName, "CyclomaticComplexityMetric", 3));
}

TEST(CC_IfConstructionTest, CC_IfConstructionTest) {
    const std::string fileName = "if.py";
    EXPECT_TRUE(CheckMetric<CyclomaticComplexityMetric>(fileName, "CyclomaticComplexityMetric", 2));
}

TEST(CC_LoopsConstructionTest, CC_LoopsConstructionTest) {
    const std::string fileName = "loops.py";
    EXPECT_TRUE(CheckMetric<CyclomaticComplexityMetric>(fileName, "CyclomaticComplexityMetric", 4));
}

TEST(CC_ManyLinesTest, CC_ManyLinesTest) {
    // cc == 2 так как в функции стоит assert, нужно ли его учитывть, так как кроме него нечего нету?
    const std::string fileName = "many_lines.py";
    EXPECT_TRUE(CheckMetric<CyclomaticComplexityMetric>(fileName, "CyclomaticComplexityMetric", 2));
}

TEST(CC_ManyParametersTest, CC_ManyParametersTest) {
    // cc == 2 так как в конце функции стоит assert, нужно ли его учитывть в конце функции?
    const std::string fileName = "many_parameters.py";
    EXPECT_TRUE(CheckMetric<CyclomaticComplexityMetric>(fileName, "CyclomaticComplexityMetric", 2));
}

TEST(CC_MatchCaseTest, CC_MatchCaseTest) {
    const std::string fileName = "match_case.py";
    EXPECT_TRUE(CheckMetric<CyclomaticComplexityMetric>(fileName, "CyclomaticComplexityMetric", 3));
}

}  // namespace analyser::metric::metric_impl
