#include "metric_impl/naming_style.hpp"
#include "utils.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

// NS - Naming Style
TEST(NS_SingleLineCommentTest, NS_SingleLineCommentTest) {
    const std::string fileName = "comments.py";
    EXPECT_TRUE(CheckMetric<NamingStyleMetric>(fileName, "NamingStyleMetric", std::string("unknown")));
}

TEST(NS_MultiLinesCommentTest, NS_MultiLinesCommentTest) {
    const std::string fileName = "multi_lines_comments.py";
    EXPECT_TRUE(CheckMetric<NamingStyleMetric>(fileName, "NamingStyleMetric", std::string("unknown")));
}

TEST(NS_IfConstructionTest, NS_IfConstructionTest) {
    const std::string fileName = "if.py";
    EXPECT_TRUE(CheckMetric<NamingStyleMetric>(fileName, "NamingStyleMetric", std::string("camel case")));
}

TEST(NS_LoopsConstructionTest, NS_LoopsConstructionTest) {
    const std::string fileName = "loops.py";
    EXPECT_TRUE(CheckMetric<NamingStyleMetric>(fileName, "NamingStyleMetric", std::string("Pascal case")));
}

TEST(NS_ManyLinesTest, NS_ManyLinesTest) {
    const std::string fileName = "many_lines.py";
    EXPECT_TRUE(CheckMetric<NamingStyleMetric>(fileName, "NamingStyleMetric", std::string("lowercase")));
}

TEST(NS_ManyParametersTest, NS_ManyParametersTest) {
    const std::string fileName = "many_parameters.py";
    EXPECT_TRUE(CheckMetric<NamingStyleMetric>(fileName, "NamingStyleMetric", std::string("unknown")));
}

TEST(NS_MatchCaseTest, NS_MatchCaseTest) {
    const std::string fileName = "match_case.py";
    EXPECT_TRUE(CheckMetric<NamingStyleMetric>(fileName, "NamingStyleMetric", std::string("unknown")));
}

TEST(NS_NestedIfTest, NS_NestedIfTest) {
    const std::string fileName = "nested_if.py";
    EXPECT_TRUE(CheckMetric<NamingStyleMetric>(fileName, "NamingStyleMetric", std::string("Pascal case")));
}

TEST(NS_SimpleTest, NS_SimpleTest) {
    const std::string fileName = "simple.py";
    EXPECT_TRUE(CheckMetric<NamingStyleMetric>(fileName, "NamingStyleMetric", std::string("snake case")));
}

TEST(NS_TernaryTest, NS_TernaryTest) {
    const std::string fileName = "ternary.py";
    EXPECT_TRUE(CheckMetric<NamingStyleMetric>(fileName, "NamingStyleMetric", std::string("unknown")));
}
// здесь ваш код

}  // namespace analyser::metric::metric_impl
