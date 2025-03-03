#pragma once
#include <variant>
#include <vector>
#include "operation.h"
#include "stack.h"
#include "table.h"

namespace intr {
    using program = std::vector<op>;

    class expression_interpreter final {
    private:
        const Aggregator* mAggregator;
        Stack<op> mStack; // TODO: Вообще элементы не совсем op.. только значения
    public:
        expression_interpreter(const Aggregator* aggregator) : mAggregator(aggregator) {
            if (aggregator == nullptr) {
                throw std::runtime_error(__FUNCTION__ ": aggregator was null.");
            }
        }

        /// @brief Выполнить программу, сгенерированную парсером
        /// @param program программа для выполнения
        /// @return полином-результат или ошибка-строка
        std::variant<polynomial, std::string> execute(const std::vector<program>& program);
    };
}