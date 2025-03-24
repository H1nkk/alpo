#pragma once
#include <variant>
#include <vector>
#include "operation.h"
#include "stack.h"
#include "table.h"

namespace Intr {
    using Program = std::vector<Op>;

    class ExpressionInterpreter final {
    private:
        const Aggregator* mAggregator;
    public:
        ExpressionInterpreter(const Aggregator* aggregator) : mAggregator(aggregator) {
            if (aggregator == nullptr) {
                throw std::runtime_error(__FUNCTION__ ": aggregator was null.");
            }
        }

        /// @brief Выполнить программу, сгенерированную парсером
        /// @param program программа для выполнения
        /// @return полином-результат или ошибка-строка
        std::variant<polynomial, std::string> execute(const Program& program);
    };
}