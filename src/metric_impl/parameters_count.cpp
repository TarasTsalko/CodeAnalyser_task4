#include "metric_impl/parameters_count.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "utils.hpp"

namespace analyser::metric::metric_impl {

MetricResult::ValueType CountParametersMetric::CalculateImpl(const function::Function &f) const {

    const size_t MAX_VALUE = std::numeric_limits<size_t>::max();
    size_t prevEndPos = MAX_VALUE;
    bool paramatersSectionStarted = false;
    bool bodyOfFunctionStarted = false;

    // вспомогательная lambda
    auto get_second_value = [](auto &&part) {
        auto component = part | std::views::split(',') | std::views::drop(1) | std::views::take(1) | std::views::join |
                         std::views::drop_while([](char c) { return c == ' ' || c == '\t'; })  // drop spaces
                         | std::views::take_while([](char c) { return std::isdigit(static_cast<unsigned char>(c)); });
        return std::string_view(&*component.begin(), std::ranges::distance(component));
    };

    auto checker = [&](auto &&input) {
        std::string_view sv(&*input.begin(), std::ranges::distance(input));

        // проверка того, что началась секция с параметрами
        if (!paramatersSectionStarted) {
            paramatersSectionStarted = sv.contains("parameters");
            return false;
        }

        // если не началась секция с параметрами, выходим из функции
        if (!paramatersSectionStarted)
            return false;

        // все ast-дерево функции проверять не обязательно
        // если найден таг body или return_type, если определение функции имеет следующий вид
        // def process(self, data: List[Union[int, float]]) -> List[float]:
        if (bodyOfFunctionStarted)
            return false;
        bodyOfFunctionStarted = sv.contains("body") || sv.contains("return_type");

        // параметры функции начинаются в ast-дереве со слдеующих тегов
        // identifier - параметр
        // typed_parameter - параметр с указанием типа (param:int)
        // typed_default_parameter - параметр с указанием типа (multiplier: float = 1.0)
        if (!sv.contains("identifier") && !sv.contains("typed_parameter") && !sv.contains("typed_default_parameter"))
            return false;

        // из каждой строки получаеи [3, 23] - [3, 33]
        // и из этх данных получаем номер столбца в страке на котором параметр оканчивается
        auto parts = sv | std::views::split('[') | std::views::drop(1) | std::views::take(2);
        auto iter = std::next(parts.begin());
        auto val_str = get_second_value(*iter);
        size_t currentEndtPos = static_cast<size_t>(ToInt(val_str));

        // "встретили первый параметр функции"
        if (prevEndPos == MAX_VALUE) {
            prevEndPos = currentEndtPos;
            return true;
        }

        //  все строки в ast-дереве относятся к тому же параметру
        if (currentEndtPos <= prevEndPos)
            return false;
        // новый prevEndPos (начался новый парамет)
        prevEndPos = currentEndtPos;
        return true;
    };
    auto parts = f.ast | std::views::split('\n');
    int res = std::ranges::count_if(parts, checker);
    return res;
}

std::string CountParametersMetric::Name() const { return std::string("CountParametersMetric"); }

}  // namespace analyser::metric::metric_impl
