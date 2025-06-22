#include "metric_impl/code_lines_count.hpp"
#include "utils.hpp"

#include <memory>
#include <ranges>
#include <vector>

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

TEST(SingleLineCommentTest, SingleLineCommentTest) {
    const std::string fileName = "comments.py";
    EXPECT_TRUE(CheckMetric<CodeLinesCountMetric>(fileName, "CodeLinesCountMetric", 3));
}

TEST(MultiLinesCommentTest, MultiLinesCommentTest) {
    const std::string fileName = "multi_lines_comments.py";
    EXPECT_TRUE(CheckMetric<CodeLinesCountMetric>(fileName, "CodeLinesCountMetric", 3));
}

TEST(ExceptionConstructionTest, ExceptionConstructionTest) {
    const std::string fileName = "exceptions.py";
    EXPECT_TRUE(CheckMetric<CodeLinesCountMetric>(fileName, "CodeLinesCountMetric", 7));
}

TEST(IfConstructionTest, IfConstructionTest) {
    const std::string fileName = "if.py";
    EXPECT_TRUE(CheckMetric<CodeLinesCountMetric>(fileName, "CodeLinesCountMetric", 3));
}

TEST(LoopsConstructionTest, LoopsConstructionTest) {
    const std::string fileName = "loops.py";
    EXPECT_TRUE(CheckMetric<CodeLinesCountMetric>(fileName, "CodeLinesCountMetric", 6));
}

TEST(ManyLinesTest, ManyLinesTest) {
    const std::string fileName = "many_lines.py";
    EXPECT_TRUE(CheckMetric<CodeLinesCountMetric>(fileName, "CodeLinesCountMetric", 11));
}

TEST(ManyParametersTest, ManyParametersTest) {
    const std::string fileName = "many_parameters.py";
    EXPECT_TRUE(CheckMetric<CodeLinesCountMetric>(fileName, "CodeLinesCountMetric", 1));
}

TEST(MatchCaseTest, MatchCaseTest) {
    const std::string fileName = "match_case.py";
    EXPECT_TRUE(CheckMetric<CodeLinesCountMetric>(fileName, "CodeLinesCountMetric", 7));
}

TEST(NestedIfTest, NestedIfTest) {
    const std::string fileName = "nested_if.py";
    EXPECT_TRUE(CheckMetric<CodeLinesCountMetric>(fileName, "CodeLinesCountMetric", 8));
}

TEST(SimpleTest, SimpleTest) {
    const std::string fileName = "simple.py";
    EXPECT_TRUE(CheckMetric<CodeLinesCountMetric>(fileName, "CodeLinesCountMetric", 5));
}

TEST(TernaryTest, TernaryTest) {
    const std::string fileName = "ternary.py";
    EXPECT_TRUE(CheckMetric<CodeLinesCountMetric>(fileName, "CodeLinesCountMetric", 1));
}

}  // namespace analyser::metric::metric_impl
