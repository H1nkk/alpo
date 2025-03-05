#pragma once
#include <variant>
#include <vector>
#include "lexer/lexer_token.h"
#include "expression_interpreter/expression_interpreter.h"
#include "polynomial.h"
#include "syntax_error.h"

namespace compiler {
    class expression_compiler final {
    public:
        /// @brief Преобразовать последовательность токенов в программу для интерпретации
        /// @param tokens Последовательность токенов, которую нужно скомпилировать
        /// @return Программа или синтаксическая ошибка
        std::variant<intr::program, syntax_error> compile_expression(const std::vector<lexer::token>& tokens);
    };
}