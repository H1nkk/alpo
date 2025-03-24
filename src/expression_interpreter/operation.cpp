#include "expression_interpreter/operation.h"

namespace Intr {
    std::string toString(Opcode opc)
    {
        switch (opc)
        {
        case Opcode::ADD: return "Opcode::ADD";
        default: return "UNKNOWN";
        }
    }
}