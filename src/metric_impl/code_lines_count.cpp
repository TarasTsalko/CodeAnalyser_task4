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

std::pair<int, int> GetRange(std::string_view sv) {
    auto first = sv |
                 std::views::drop_while([](char c) { return c != '['; })     // Отбрасываем первую часть (до первой '[')
                 | std::views::drop(1)                                       // отбросили [
                 | std::views::take_while([](char c) { return c != ','; });  // берем часть до первой ','

    auto second = sv |
                  std::views::drop_while([](char c) { return c != '['; })  // Отбрасываем первую часть (до первой '[')
                  | std::views::drop(1)                                    // отбросили [
                  | std::views::drop_while([](char c) {
                        return c != '[';
                    })                   // (у второй группы чисел) Отбрасываем первую часть (до первой '[')
                  | std::views::drop(1)  // отбросили [
                  | std::views::take_while([](char c) { return c != ']'; })   // взяли все до ']' у второй группы чисел
                  | std::views::take_while([](char c) { return c != ','; });  // взяли все до запятой

    std::string_view start_pos_sv(&*first.begin(), std::ranges::distance(first));
    std::string_view end_pos_sv(&*second.begin(), std::ranges::distance(second));
    return std::make_pair(ToInt(start_pos_sv), ToInt(end_pos_sv));
}

MetricResult::ValueType CodeLinesCountMetric::CalculateImpl(const function::Function &f) const {

    int prevStartPos = -1, prevEndPos = -1;
    int operationCount = 0;
    bool bodyOfFunctionStarted = false;
    auto checker = [&](auto &&part) {
        std::string_view sv(&*part.begin(), std::ranges::distance(part));

        // Строку с прототипом функции не учитываем (уточнил у куратора)
        if (!bodyOfFunctionStarted) {
            bodyOfFunctionStarted = sv.contains("body");
            return;
        }

        // подсчитывам строки кода только самого тела функции,
        // без строки объявления функции (реализовыва оба варианта, этот правельный, уточнял у куратора)
        if (!bodyOfFunctionStarted)
            return;

        // "служебный" тэг, его тоже пропускаем, так как за ним стоит определенный тэг
        // string на пример, если это многострочный комментарий
        if (sv.contains("expression_statement"))
            return;
        // отбрасываем комментарии
        if (sv.contains("comment") || sv.contains("string"))
            return;

        // берем диапозон чисел отвечающих за строку и высчитывам сколько строк занимает действие
        auto [first, second] = GetRange(sv);
        if (prevEndPos < second) {
            prevStartPos = first;
            prevEndPos = second;
            operationCount += second - first + 1;
        }
    };

    auto parts = f.ast | std::views::split('\n');
    std::ranges::for_each(parts, checker);
    return operationCount;
}

std::string CodeLinesCountMetric::Name() const { return std::string("CodeLinesCountMetric"); }

}  // namespace analyser::metric::metric_impl
