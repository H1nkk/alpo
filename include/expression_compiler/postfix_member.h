#pragma once

#include "lexer/lexer_token.h"

namespace Compiler {
    class PostfixMember
    {
    public:
        explicit PostfixMember(Lexer::Token token = {}, int precedence = 0, bool prefixOp = false) :
            mToken(token), mPrecedence(precedence), mPrefixOp(prefixOp)
        {}

        Lexer::Token token() const noexcept { return mToken; }
        Lexer::TokenType type() const noexcept { return mToken.type(); }
        int precedence() const noexcept { return mPrecedence; }
        bool isPrefixOp() const noexcept { return mPrefixOp; }

    private:
        Lexer::Token mToken;
        int mPrecedence;
        bool mPrefixOp;
    };
}