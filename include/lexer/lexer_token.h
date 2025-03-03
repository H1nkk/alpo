#pragma once
#include <string>

namespace lexer {
    enum class token_type
    {
        NONE,
        NUM, ID, LPAR, RPAR,
        PLUS, MINUS, MULT, CARET,
        ENDOFFILE
    };

    std::string to_string(token_type type);

    class token final
    {
    public:
        token(token_type type = token_type::NONE, const std::string& value = "",
            size_t start = 0, size_t end = 0) 
            : mType(type), mValue(value), mStart(start), mEnd(end) {}

        token_type type() const { return mType; }
        std::string value() const { return mValue; }
        size_t startPos() const { return mStart; }
        size_t endPos() const { return mEnd; }

    private:
        token_type mType;
        std::string mValue;
        size_t mStart, mEnd;
    };
}