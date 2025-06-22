#include "metric_impl/cyclomatic_complexity.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <flat_set>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyser::metric::metric_impl {

MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function &f) const {

    // Вопросы к ревьюверу:
    // 1. Как я понимаю цикломатическая сложность отдельно взятой функции ( количеств принятия решений + 1 )?
    // 2. Составной if повышает цикломатическую сложность на количество составных частей прим: if a > 0.0 or a < -2.0
    // такой if повысит её не на 1, а на 2?
    // 3. else не увеличивает цикломатическую сложность, только if, else_if, как и finaly(не увеличивает сложность),
    // тогда зачем они упоминаются в пункте задания?
    // 4. нужно ли учитывть конструкции вида  self.data = [item for item in self.data if item is not None]?
    // сейчас учитывается if в таких конструкциях, но не for

    // flat_set - не большой размер, частый поиск
    const std::flat_set<std::string> significantLines = {
        "if_statement",    "if_clause",       "for_statement",          "elif_clause",
        "except_clause",   "case_pattern",    "conditional_expression", "boolean_operator",
        "while_statement", "assert_statement"};

    bool caseBlockStarted = false;
    auto checker = [&significantLines, &caseBlockStarted](auto &&part) {
        std::string_view sv(&*part.begin(), std::ranges::distance(part));
        const bool res = std::ranges::any_of(significantLines,
                                             [&sv](const std::string &s) { return sv.find(s) != std::string::npos; });
        if (!res && !caseBlockStarted)
            return false;

        // если нашли важную для цикломотической сложности конструкцию,
        // то проверяем, что это case, поднимаем флаг caseBlockStarted
        if (!caseBlockStarted && sv.contains("case_pattern")) {
            caseBlockStarted = true;
            return false;
        }

        if (caseBlockStarted) {
            caseBlockStarted = false;
            // если после case_pattern идет сразу consequence-блок, то это case _
            if (sv.contains("consequence"))
                return false;  // это case _:, default-ветка не увеличивающая сложности
            else
                return true;
        }
        return res;
    };
    auto parts = f.ast | std::views::split('\n');
    int res = std::ranges::count_if(parts, checker) + 1;
    return res;
}

std::string CyclomaticComplexityMetric::Name() const { return std::string("CyclomaticComplexityMetric"); }

}  // namespace analyser::metric::metric_impl
