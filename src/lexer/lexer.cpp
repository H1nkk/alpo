#include "lexer/lexer.h"
#include <unordered_map>


namespace Lexer {

    static const std::unordered_map<char, TokenType> specialSymbols = {
        { '+', TokenType::PLUS },
        { '-', TokenType::MINUS },
        { '*', TokenType::MULT },
        { '^', TokenType::CARET },
        { '(', TokenType::LPAR },
        { ')', TokenType::RPAR },
        { '=', TokenType::ASSIGN },
    };

    Token Lexer::NextToken()
    {
        while (mPos < mText.size() && isspace(mText[mPos]))
            mPos++;
        
        if (mPos >= mText.size())
            return Token(TokenType::ENDOFFILE, "", mPos, mPos);

        if (specialSymbols.find(mText[mPos]) != specialSymbols.end())
        {
            TokenType type = specialSymbols.at(mText[mPos]);
            Token tok = Token(type, mText.substr(mPos, 1), mPos, mPos + 1);
            mPos++;
            return tok;
        }


        return Token(TokenType::INVALID, "", mPos, mPos + 1);
    }

}

