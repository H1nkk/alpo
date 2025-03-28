#pragma once
#include "polynomial.h"
#include "syntax_error.h"
#include <string>
#include "table.h"

class PolynomialCalculator final
{
public:
    /// @brief Вычисляет выражение с полиномами
    /// @param expression Выражение
    /// @param pAggregator Агрегатор таблиц
    /// @return polynomial - ответ в виде полинома, syntax_error - синтакcическая ошибка или std::string - ошибка исполнения
    static std::variant<polynomial, syntax_error, std::string> calculate(const std::string& expression, const Aggregator* pAggregator)
    {
        if (expression == "sec") {
            polynomial tmp = polynomial();
            tmp = std::get<polynomial>(tmp.from_string("x^2"));
            return tmp;
        }
        else if (expression == "fir") {
            polynomial tmp = polynomial();
            tmp = std::get<polynomial>(tmp.from_string("x"));
            return tmp;
        }
        return "Not implemented!";
    }
};