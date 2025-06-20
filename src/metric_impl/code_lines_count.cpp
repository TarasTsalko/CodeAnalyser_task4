#include "metric_impl/code_lines_count.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "utils.hpp"

namespace analyser::metric::metric_impl {

MetricResult::ValueType CodeLinesCountMetric::CalculateImpl(const function::Function &f) const {
    const size_t MAX_VALUE = std::numeric_limits<size_t>::max();
    size_t prevPos = MAX_VALUE;

    auto checker = [&prevPos](auto &&part) {
        std::string_view sv(&*part.begin(), std::ranges::distance(part));

        // считаем количество линий в коде функции вместе с заголовкрм функции
        // пропускаем:
        // "comment" - однострочные коментарии
        //  "expression_statement" - за "expression_statement" может идти не только действие, но и "string",
        //  то есть многострочный комментарий
        //  body - служебный тэг, его тоже пропускаем
        if (sv.contains("comment") || sv.contains("expression_statement") || sv.contains("string") ||
            sv.contains("body"))
            return false;

        // получаем индек начала строки без использования циклов
        auto first_number = sv | std::views::split('[')  // разделяем по '['
                            | std::views::drop(1)        // Отбрасываем первую часть (до первой '[')
                            | std::views::take(1)        // Берем фрагмент после '['
                            | std::views::join  // Получившиеся чфрагменты (после '[' объединяем строку для take_while )
                            | std::views::take_while([](char c) { return c != ','; });  // берем часть до первой ','

        // преобразовывем строку в числоW
        std::string_view start_pos_sv(&*first_number.begin(), std::ranges::distance(first_number));
        const int currentPos = ToInt(start_pos_sv);
        // запоминаем текущую позицию
        if (prevPos == MAX_VALUE) {
            prevPos = currentPos;
            return true;
        }

        // если номер строки в скобках не изменился, то не считаем её повторно (блок дерева продалжается)
        // строки виде a = 5; b = 7; (в AST-дереве отображаются несколькими строками)
        if (prevPos == currentPos)
            return false;
        prevPos = currentPos;
        return true;
    };

    auto parts = f.ast | std::views::split('\n');
    return std::ranges::count_if(parts, checker);
}

std::string CodeLinesCountMetric::Name() const { return std::string("CodeLinesCountMetric"); }

}  // namespace analyser::metric::metric_impl
