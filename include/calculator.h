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
    std::variant<polynomial, syntax_error, std::string> calculate(const std::string& expression, const Aggregator* pAggregator)
    {
        return "Not implemented!";
    }
};