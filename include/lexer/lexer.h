#pragma once
#include <string>
#include "lexer_token.h"

namespace Lexer {
    class Lexer final
    {
    private:
        std::string mText;
        size_t mPos;
        
    public:
        /// @param str Выражение для токенизации
        Lexer(const std::string& str) : mText(str), mPos(0) {}

        /// @brief Получить следующий токен из строки
        Token NextToken();
    };
}