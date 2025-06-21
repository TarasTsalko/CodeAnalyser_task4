#include "metric_impl/code_lines_count.hpp"
#include "metric.hpp"

#include "file.hpp"
#include "function.hpp"

#include <memory>
#include <ranges>
#include <vector>

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

auto CreateFunctions(const std::string &fileName) {
    using namespace analyser;
    const file::File sourceFile(fileName);
    function::FunctionExtractor extructor;
    auto functions = extructor.Get(sourceFile);
    return functions;
}

bool CheckMetric(const std::string &fileName, int val) {
    auto functions = CreateFunctions(fileName);
    std::unique_ptr<metric::IMetric> metric = std::make_unique<CodeLinesCountMetric>();
    return std::ranges::all_of(functions, [&metric, val](const auto &f) {
        const auto metricRes = metric->Calculate(f);
        return "CodeLinesCountMetric" == metricRes.metric_name && metricRes.value == val;
    });
}

TEST(SingleLineCommentTest, SingleLineCommentTest) {
    const std::string fileName = "comments.py";
    EXPECT_TRUE(CheckMetric(fileName, 3));
}

TEST(MultiLinesCommentTest, MultiLinesCommentTest) {
    const std::string fileName = "multi_lines_comments.py";
    EXPECT_TRUE(CheckMetric(fileName, 3));
}

TEST(ExceptionConstructionTest, ExceptionConstructionTest) {
    const std::string fileName = "exceptions.py";
    EXPECT_TRUE(CheckMetric(fileName, 7));
}

TEST(IfConstructionTest, IfConstructionTest) {
    const std::string fileName = "if.py";
    EXPECT_TRUE(CheckMetric(fileName, 3));
}

TEST(LoopsConstructionTest, LoopsConstructionTest) {
    const std::string fileName = "loops.py";
    EXPECT_TRUE(CheckMetric(fileName, 6));
}

TEST(ManyLinesTest, ManyLinesTest) {
    const std::string fileName = "many_lines.py";
    EXPECT_TRUE(CheckMetric(fileName, 11));
}

TEST(ManyParametersTest, ManyParametersTest) {
    const std::string fileName = "many_parameters.py";
    EXPECT_TRUE(CheckMetric(fileName, 1));
}

TEST(MatchCaseTest, MatchCaseTest) {
    const std::string fileName = "match_case.py";
    EXPECT_TRUE(CheckMetric(fileName, 7));
}

TEST(NestedIfTest, NestedIfTest) {
    const std::string fileName = "nested_if.py";
    EXPECT_TRUE(CheckMetric(fileName, 8));
}

TEST(SimpleTest, SimpleTest) {
    const std::string fileName = "simple.py";
    EXPECT_TRUE(CheckMetric(fileName, 5));
}

TEST(TernaryTest, TernaryTest) {
    const std::string fileName = "ternary.py";
    EXPECT_TRUE(CheckMetric(fileName, 1));
}

}  // namespace analyser::metric::metric_impl
