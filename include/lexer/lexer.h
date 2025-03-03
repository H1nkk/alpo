#pragma once
#include <string>
#include "lexer_token.h"

namespace lexer {
    class lexer final
    {
    public:
        /// @param str Выражение для токенизации
        lexer(const std::string& str);

        /// @brief Получить следующий токен из строки
        token NextToken();
    };
}