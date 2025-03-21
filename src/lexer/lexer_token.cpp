#include "lexer/lexer_token.h"

namespace Lexer {

    std::string toString(TokenType type)
    {
        switch (type)
        {
        case TokenType::NONE: return "TokenType::NONE";
        case TokenType::NUM: return "TokenType::NUM";
        case TokenType::INT: return "TokenType::INT";
        case TokenType::ID: return "TokenType::ID";
        case TokenType::LPAR: return "TokenType::LPAR";
        case TokenType::RPAR: return "TokenType::RPAR";
        case TokenType::X: return "TokenType::X";
        case TokenType::y: return "TokenType::Y";
        case TokenType::Z: return "TokenType::Z";
        case TokenType::w: return "TokenType::W";
        case TokenType::PLUS: return "TokenType::PLUS";
        case TokenType::MINUS: return "TokenType::MINUS";
        case TokenType::MULT: return "TokenType::MULT";
        case TokenType::CARET: return "TokenType::CARET";
        case TokenType::INVALID: return "TokenType::INVALID";
        case TokenType::ASSIGN: return "TokenType::ASSIGN";
        case TokenType::ENDOFFILE: return "TokenType::ENDOFFILE";
        default: return "UNKNOWN";
        }
    }

}