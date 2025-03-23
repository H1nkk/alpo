#pragma once
#include <variant>
#include <string>
#include "polynomial.h"

namespace intr {
    enum class opcode {
        ASSIGN,
        ADD, SUBTRACT, MULT, POWER, UMINUS,
        CALC, DERX, DERY, DERZ, DERW,
        INTX, INTY, INTZ, INTW
    };

    // Код операции/полином/идентификатор/число
    using op = std::variant<opcode, polynomial, std::string, double, unsigned long>;

    std::string toString(opcode opcode);
}