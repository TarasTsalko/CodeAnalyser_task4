#include "metric_impl/naming_style.hpp"

#include <unistd.h>

#include <algorithm>
#include <ranges>

namespace analyser::metric::metric_impl {

bool isCamelCase(const std::string &str) {

    if (str.empty() || std::isupper(str[0]))
        return false;

    // первый символ не заглавный
    auto view = str | std::views::drop(1);

    // Один символ обязательно заглавный, осттольные любые
    return std::ranges::any_of(view, [](char ch) { return std::isupper(ch); }) &&
           std::ranges::all_of(view, [](char ch) { return std::islower(ch) || std::isupper(ch); });
}

bool isSnakeCase(const std::string &str) {
    if (str.empty())
        return false;

    // Проверяем, что все символы либо строчные, либо подчёркивания
    auto isValidChar = [](char ch) { return std::islower(ch) || ch == '_'; };

    // Проверяем базовые условия
    if (!std::ranges::all_of(str, isValidChar))
        return false;

    // Проверяем, что подчёркивания не стоят в начале/конце
    if (str.front() == '_' || str.back() == '_')
        return false;

    // Проверяем наличие хотя бы одного подчеркивания
    if (std::ranges::none_of(str, [](char ch) { return ch == '_'; }))
        return false;

    // Проверяем, что нет нескольких подчёркиваний подряд
    auto hasMultipleUnderscores = [](char ch1, char ch2) { return ch1 == '_' && ch2 == '_'; };

    // std::views::slide(2) - "скользящее окно" в 2 символа, чтобы проверить, что нет двух подряд "__"
    // таким оброзом проверим, что есть хотябы один символ не _
    return !std::ranges::any_of(str | std::views::slide(2), [&hasMultipleUnderscores](const auto &pair) {
        return hasMultipleUnderscores(pair[0], pair[1]);
    });
}

bool isPascalCase(const std::string &str) {
    if (str.empty())
        return false;

    // Проверяем, что первый символ заглавный
    if (!std::isupper(str[0]))
        return false;

    // Проверяем, что все символы либо строчные, либо заглавные
    auto isValidChar = [](char ch) { return std::islower(ch) || std::isupper(ch); };

    if (!std::ranges::all_of(str, isValidChar))
        return false;

    // Проверяем, что после каждой заглавной буквы следует строчная
    // или конец строки
    auto isValidTransition = [](char ch1, char ch2) { return !(std::isupper(ch1) && std::isupper(ch2)); };

    return std::ranges::all_of(str | std::views::slide(2),
                               [&isValidTransition](const auto &pair) { return isValidTransition(pair[0], pair[1]); });
}

bool isLowerCase(const std::string &str) {
    if (str.empty())
        return false;

    return std::ranges::all_of(str, [](char ch) { return std::islower(ch) || std::isdigit(ch); });
}

MetricResult::ValueType NamingStyleMetric::CalculateImpl(const function::Function &f) const {

    // Вопрос к ревьюверу: служебные функции Python
    // прим.: __init__, определяются как unknown
    // так как это  Dunder от Double Underscore, такого стиля
    // в описании задания нет. Это ошибка в задании>
    if (isCamelCase(f.name))
        return "camel case";
    else if (isSnakeCase(f.name))
        return "snake case";
    else if (isPascalCase(f.name))
        return "Pascal case";
    else if (isLowerCase(f.name))
        return "lowercase";

    return "unknown";
}

std::string NamingStyleMetric::Name() const { return std::string("NamingStyleMetric"); }

}  // namespace analyser::metric::metric_impl
