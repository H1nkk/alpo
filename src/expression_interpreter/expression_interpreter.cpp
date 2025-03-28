#include "expression_interpreter/expression_interpreter.h"
#include "table.h"

namespace Intr {

    class VirtualMachine final
    {
    private:
        Aggregator* pAggregator;
        Stack<Op> mOperands;

        double getDoubleOp()
        {
            Op op = mOperands.Top();
            mOperands.Pop();

            if (std::holds_alternative<unsigned long>(op))
            {
                return std::get<unsigned long>(op);
            } else if (std::holds_alternative<double>(op))
            {
                return std::get<double>(op);
            } else
            {
                throw std::runtime_error(__FUNCTION__ ": unknown operand type.");
            }
        }

        polynomial getPolynomialOp()
        {
            Op op = mOperands.Top();
            mOperands.Pop();

            if (std::holds_alternative<polynomial>(op))
            {
                return std::get<polynomial>(op);
            }
            else if (std::holds_alternative<unsigned long>(op))
            {
                return std::get<polynomial>(polynomial::from_string(std::to_string(std::get<unsigned long>(op))));
            }
            else if (std::holds_alternative<double>(op))
            {
                return std::get<polynomial>(polynomial::from_string(std::to_string(std::get<double>(op))));
            }
            else if (std::holds_alternative<std::string>(op))
            {
                auto p = pAggregator->findPolynomial(std::get<std::string>(op));
                if (!p.has_value())
                {
                    throw "Polynomial with name " + std::get<std::string>(op) + " does not exist";
                }

                return p.value();
            }
            else
            {
                throw std::runtime_error(__FUNCTION__ ": unknown operand type.");
            }
        }

        void add()
        {
            polynomial p1 = getPolynomialOp();
            polynomial p2 = getPolynomialOp();
            mOperands.Push(p2 + p1);
        }

        void subtract()
        {
            polynomial p1 = getPolynomialOp();
            polynomial p2 = getPolynomialOp();
            mOperands.Push(p2 - p1);
        }

        void multiply()
        {
            polynomial p1 = getPolynomialOp();
            polynomial p2 = getPolynomialOp();
            mOperands.Push(p2 * p1);
        }

        void assign()
        {
            polynomial p = getPolynomialOp();
            
            Op name = mOperands.Top();
            mOperands.Pop();

            if (!std::holds_alternative<std::string>(name))
            {
                throw std::runtime_error(__FUNCTION__ ": expected identifier.");
            }

            mOperands.Push(p);
            pAggregator->addPolynomial(std::get<std::string>(name), p);
        }

        void negate()
        {
            Op op = mOperands.Top();
            mOperands.Pop();

            if (std::holds_alternative<polynomial>(op))
            {
                mOperands.Push(-std::get<polynomial>(op));
            } else if (std::holds_alternative<unsigned long>(op))
            {
                mOperands.Push(-(double)std::get<unsigned long>(op));
            } else if (std::holds_alternative<double>(op))
            {
                mOperands.Push(-std::get<double>(op));
            } else if (std::holds_alternative<std::string>(op))
            {
                auto p = pAggregator->findPolynomial(std::get<std::string>(op));
                if (!p.has_value())
                {
                    throw "Polynomial with name " + std::get<std::string>(op) + " does not exist";
                }

                mOperands.Push(-p.value());
            } else
            {
                throw std::runtime_error(__FUNCTION__ ": unknown operand type.");
            }
        }

        void calc()
        {
            double w = getDoubleOp();
            double z = getDoubleOp();
            double y = getDoubleOp();
            double x = getDoubleOp();
            polynomial p = getPolynomialOp();
            mOperands.Push(p.evaluate(w, x, y, z));
        }

        void derx()
        {
            mOperands.Push(getPolynomialOp().derivative_x());
        }

        void dery()
        {
            mOperands.Push(getPolynomialOp().derivative_y());
        }

        void derz()
        {
            mOperands.Push(getPolynomialOp().derivative_z());
        }

        void derw()
        {
            mOperands.Push(getPolynomialOp().derivative_w());
        }

        void intx()
        {
            mOperands.Push(getPolynomialOp().integral_x());
        }

        void inty()
        {
            mOperands.Push(getPolynomialOp().integral_y());
        }

        void intz()
        {
            mOperands.Push(getPolynomialOp().integral_z());
        }

        void intw()
        {
            mOperands.Push(getPolynomialOp().integral_w());
        }

    public:
        VirtualMachine(Aggregator* aggregator) : pAggregator(aggregator) {}

        void execOp(const Op& op)
        {
            if (!std::holds_alternative<Opcode>(op))
            {
                mOperands.Push(op);
                return;
            }

            Opcode opcode = std::get<Opcode>(op);
            switch (opcode)
            {
            case Opcode::ADD: add(); break;
            case Opcode::SUBTRACT: subtract(); break;
            case Opcode::MULT: multiply(); break;
            case Opcode::UMINUS: negate(); break;
            case Opcode::ASSIGN: assign(); break;
            case Opcode::CALC: calc(); break;
            case Opcode::DERX: derx(); break;
            case Opcode::DERY: dery(); break;
            case Opcode::DERZ: derz(); break;
            case Opcode::DERW: derw(); break;
            case Opcode::INTX: intx(); break;
            case Opcode::INTY: inty(); break;
            case Opcode::INTZ: intz(); break;
            case Opcode::INTW: intw(); break;
            default: throw std::runtime_error(__FUNCTION__ ": operation is not supported.");
            }
        }

        polynomial getResult()
        {
            return getPolynomialOp();
        }
    };

    std::variant<polynomial, std::string> ExpressionInterpreter::execute(const Program& program)
    {
        VirtualMachine vm(mAggregator);

        for (size_t i = 0; i < program.size(); ++i)
        {
            vm.execOp(program[i]);
        }

        return vm.getResult();
    }
}