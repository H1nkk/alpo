#include "expression_compiler/expression_compiler.h"
#include "expression_compiler/postfix_member.h"
#include "lexer/lexer_token.h"
#include "stack.h"
#include <unordered_set>

namespace Compiler {
    
    using Lexer::TokenType;
    using Lexer::Token;

    enum class PostfixType
    {
        INT, ID, POLYNOMIAL
    };

    struct TokenTypePairHasher
    {
        size_t operator()(std::pair<TokenType, TokenType> t) const
        {
            return static_cast<size_t>(t.first) * 1000 + static_cast<size_t>(t.second);
        }
    };

    const static std::unordered_set<std::pair<TokenType, TokenType>, TokenTypePairHasher> validTokenSequences{
        { Lexer::TokenType::NONE, Lexer::TokenType::FLOAT },
        { Lexer::TokenType::NONE, Lexer::TokenType::INT },
        { Lexer::TokenType::NONE, Lexer::TokenType::ID },
        { Lexer::TokenType::NONE, Lexer::TokenType::X },
        { Lexer::TokenType::NONE, Lexer::TokenType::Y },
        { Lexer::TokenType::NONE, Lexer::TokenType::Z },
        { Lexer::TokenType::NONE, Lexer::TokenType::W },
        { Lexer::TokenType::NONE, Lexer::TokenType::LPAR },
        { Lexer::TokenType::NONE, Lexer::TokenType::MINUS },
        { Lexer::TokenType::NONE, Lexer::TokenType::CALC },
        { Lexer::TokenType::NONE, Lexer::TokenType::DERX },
        { Lexer::TokenType::NONE, Lexer::TokenType::DERY },
        { Lexer::TokenType::NONE, Lexer::TokenType::DERZ },
        { Lexer::TokenType::NONE, Lexer::TokenType::DERW },
        { Lexer::TokenType::NONE, Lexer::TokenType::INTX },
        { Lexer::TokenType::NONE, Lexer::TokenType::INTY },
        { Lexer::TokenType::NONE, Lexer::TokenType::INTZ },
        { Lexer::TokenType::NONE, Lexer::TokenType::INTW },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::X },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::Y },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::Z },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::W },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::RPAR },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::PLUS },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::MINUS },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::MULT },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::COMMA },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::ENDOFFILE },
        { Lexer::TokenType::INT, Lexer::TokenType::X },
        { Lexer::TokenType::INT, Lexer::TokenType::Y },
        { Lexer::TokenType::INT, Lexer::TokenType::Z },
        { Lexer::TokenType::INT, Lexer::TokenType::W },
        { Lexer::TokenType::INT, Lexer::TokenType::RPAR },
        { Lexer::TokenType::INT, Lexer::TokenType::PLUS },
        { Lexer::TokenType::INT, Lexer::TokenType::MINUS },
        { Lexer::TokenType::INT, Lexer::TokenType::MULT },
        { Lexer::TokenType::INT, Lexer::TokenType::COMMA },
        { Lexer::TokenType::INT, Lexer::TokenType::ENDOFFILE },
        { Lexer::TokenType::ID, Lexer::TokenType::RPAR },
        { Lexer::TokenType::ID, Lexer::TokenType::PLUS },
        { Lexer::TokenType::ID, Lexer::TokenType::MINUS },
        { Lexer::TokenType::ID, Lexer::TokenType::MULT },
        { Lexer::TokenType::ID, Lexer::TokenType::ASSIGN },
        { Lexer::TokenType::ID, Lexer::TokenType::COMMA },
        { Lexer::TokenType::ID, Lexer::TokenType::ENDOFFILE },
        { Lexer::TokenType::X, Lexer::TokenType::INT },
        { Lexer::TokenType::X, Lexer::TokenType::X },
        { Lexer::TokenType::X, Lexer::TokenType::Y },
        { Lexer::TokenType::X, Lexer::TokenType::Z },
        { Lexer::TokenType::X, Lexer::TokenType::W },
        { Lexer::TokenType::X, Lexer::TokenType::RPAR },
        { Lexer::TokenType::X, Lexer::TokenType::PLUS },
        { Lexer::TokenType::X, Lexer::TokenType::MINUS },
        { Lexer::TokenType::X, Lexer::TokenType::MULT },
        { Lexer::TokenType::X, Lexer::TokenType::CARET },
        { Lexer::TokenType::X, Lexer::TokenType::COMMA },
        { Lexer::TokenType::X, Lexer::TokenType::ENDOFFILE },
        { Lexer::TokenType::Y, Lexer::TokenType::INT },
        { Lexer::TokenType::Y, Lexer::TokenType::X },
        { Lexer::TokenType::Y, Lexer::TokenType::Y },
        { Lexer::TokenType::Y, Lexer::TokenType::Z },
        { Lexer::TokenType::Y, Lexer::TokenType::W },
        { Lexer::TokenType::Y, Lexer::TokenType::RPAR },
        { Lexer::TokenType::Y, Lexer::TokenType::PLUS },
        { Lexer::TokenType::Y, Lexer::TokenType::MINUS },
        { Lexer::TokenType::Y, Lexer::TokenType::MULT },
        { Lexer::TokenType::Y, Lexer::TokenType::CARET },
        { Lexer::TokenType::Y, Lexer::TokenType::COMMA },
        { Lexer::TokenType::Y, Lexer::TokenType::ENDOFFILE },
        { Lexer::TokenType::Z, Lexer::TokenType::INT },
        { Lexer::TokenType::Z, Lexer::TokenType::X },
        { Lexer::TokenType::Z, Lexer::TokenType::Y },
        { Lexer::TokenType::Z, Lexer::TokenType::Z },
        { Lexer::TokenType::Z, Lexer::TokenType::W },
        { Lexer::TokenType::Z, Lexer::TokenType::RPAR },
        { Lexer::TokenType::Z, Lexer::TokenType::PLUS },
        { Lexer::TokenType::Z, Lexer::TokenType::MINUS },
        { Lexer::TokenType::Z, Lexer::TokenType::MULT },
        { Lexer::TokenType::Z, Lexer::TokenType::CARET },
        { Lexer::TokenType::Z, Lexer::TokenType::COMMA },
        { Lexer::TokenType::Z, Lexer::TokenType::ENDOFFILE },
        { Lexer::TokenType::W, Lexer::TokenType::INT },
        { Lexer::TokenType::W, Lexer::TokenType::X },
        { Lexer::TokenType::W, Lexer::TokenType::Y },
        { Lexer::TokenType::W, Lexer::TokenType::Z },
        { Lexer::TokenType::W, Lexer::TokenType::W },
        { Lexer::TokenType::W, Lexer::TokenType::RPAR },
        { Lexer::TokenType::W, Lexer::TokenType::PLUS },
        { Lexer::TokenType::W, Lexer::TokenType::MINUS },
        { Lexer::TokenType::W, Lexer::TokenType::MULT },
        { Lexer::TokenType::W, Lexer::TokenType::CARET },
        { Lexer::TokenType::W, Lexer::TokenType::COMMA },
        { Lexer::TokenType::W, Lexer::TokenType::ENDOFFILE },
        { Lexer::TokenType::LPAR, Lexer::TokenType::FLOAT },
        { Lexer::TokenType::LPAR, Lexer::TokenType::INT },
        { Lexer::TokenType::LPAR, Lexer::TokenType::ID },
        { Lexer::TokenType::LPAR, Lexer::TokenType::X },
        { Lexer::TokenType::LPAR, Lexer::TokenType::Y },
        { Lexer::TokenType::LPAR, Lexer::TokenType::Z },
        { Lexer::TokenType::LPAR, Lexer::TokenType::W },
        { Lexer::TokenType::LPAR, Lexer::TokenType::LPAR },
        { Lexer::TokenType::LPAR, Lexer::TokenType::MINUS },
        { Lexer::TokenType::LPAR, Lexer::TokenType::CALC },
        { Lexer::TokenType::LPAR, Lexer::TokenType::DERX },
        { Lexer::TokenType::LPAR, Lexer::TokenType::DERY },
        { Lexer::TokenType::LPAR, Lexer::TokenType::DERZ },
        { Lexer::TokenType::LPAR, Lexer::TokenType::DERW },
        { Lexer::TokenType::LPAR, Lexer::TokenType::INTX },
        { Lexer::TokenType::LPAR, Lexer::TokenType::INTY },
        { Lexer::TokenType::LPAR, Lexer::TokenType::INTZ },
        { Lexer::TokenType::LPAR, Lexer::TokenType::INTW },
        { Lexer::TokenType::RPAR, Lexer::TokenType::PLUS },
        { Lexer::TokenType::RPAR, Lexer::TokenType::MINUS },
        { Lexer::TokenType::RPAR, Lexer::TokenType::MULT },
        { Lexer::TokenType::RPAR, Lexer::TokenType::COMMA },
        { Lexer::TokenType::RPAR, Lexer::TokenType::ENDOFFILE },
        { Lexer::TokenType::PLUS, Lexer::TokenType::FLOAT },
        { Lexer::TokenType::PLUS, Lexer::TokenType::INT },
        { Lexer::TokenType::PLUS, Lexer::TokenType::ID },
        { Lexer::TokenType::PLUS, Lexer::TokenType::X },
        { Lexer::TokenType::PLUS, Lexer::TokenType::Y },
        { Lexer::TokenType::PLUS, Lexer::TokenType::Z },
        { Lexer::TokenType::PLUS, Lexer::TokenType::W },
        { Lexer::TokenType::PLUS, Lexer::TokenType::LPAR },
        { Lexer::TokenType::PLUS, Lexer::TokenType::CALC },
        { Lexer::TokenType::PLUS, Lexer::TokenType::DERX },
        { Lexer::TokenType::PLUS, Lexer::TokenType::DERY },
        { Lexer::TokenType::PLUS, Lexer::TokenType::DERZ },
        { Lexer::TokenType::PLUS, Lexer::TokenType::DERW },
        { Lexer::TokenType::PLUS, Lexer::TokenType::INTX },
        { Lexer::TokenType::PLUS, Lexer::TokenType::INTY },
        { Lexer::TokenType::PLUS, Lexer::TokenType::INTZ },
        { Lexer::TokenType::PLUS, Lexer::TokenType::INTW },
        { Lexer::TokenType::MINUS, Lexer::TokenType::FLOAT },
        { Lexer::TokenType::MINUS, Lexer::TokenType::INT },
        { Lexer::TokenType::MINUS, Lexer::TokenType::ID },
        { Lexer::TokenType::MINUS, Lexer::TokenType::X },
        { Lexer::TokenType::MINUS, Lexer::TokenType::Y },
        { Lexer::TokenType::MINUS, Lexer::TokenType::Z },
        { Lexer::TokenType::MINUS, Lexer::TokenType::W },
        { Lexer::TokenType::MINUS, Lexer::TokenType::LPAR },
        { Lexer::TokenType::MINUS, Lexer::TokenType::CALC },
        { Lexer::TokenType::MINUS, Lexer::TokenType::DERX },
        { Lexer::TokenType::MINUS, Lexer::TokenType::DERY },
        { Lexer::TokenType::MINUS, Lexer::TokenType::DERZ },
        { Lexer::TokenType::MINUS, Lexer::TokenType::DERW },
        { Lexer::TokenType::MINUS, Lexer::TokenType::INTX },
        { Lexer::TokenType::MINUS, Lexer::TokenType::INTY },
        { Lexer::TokenType::MINUS, Lexer::TokenType::INTZ },
        { Lexer::TokenType::MINUS, Lexer::TokenType::INTW },
        { Lexer::TokenType::MULT, Lexer::TokenType::FLOAT },
        { Lexer::TokenType::MULT, Lexer::TokenType::INT },
        { Lexer::TokenType::MULT, Lexer::TokenType::ID },
        { Lexer::TokenType::MULT, Lexer::TokenType::X },
        { Lexer::TokenType::MULT, Lexer::TokenType::Y },
        { Lexer::TokenType::MULT, Lexer::TokenType::Z },
        { Lexer::TokenType::MULT, Lexer::TokenType::W },
        { Lexer::TokenType::MULT, Lexer::TokenType::LPAR },
        { Lexer::TokenType::MULT, Lexer::TokenType::CALC },
        { Lexer::TokenType::MULT, Lexer::TokenType::DERX },
        { Lexer::TokenType::MULT, Lexer::TokenType::DERY },
        { Lexer::TokenType::MULT, Lexer::TokenType::DERZ },
        { Lexer::TokenType::MULT, Lexer::TokenType::DERW },
        { Lexer::TokenType::MULT, Lexer::TokenType::INTX },
        { Lexer::TokenType::MULT, Lexer::TokenType::INTY },
        { Lexer::TokenType::MULT, Lexer::TokenType::INTZ },
        { Lexer::TokenType::MULT, Lexer::TokenType::INTW },
        { Lexer::TokenType::CARET, Lexer::TokenType::INT },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::FLOAT },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::INT },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::ID },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::X },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::Y },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::Z },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::W },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::LPAR },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::MINUS },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::CALC },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::DERX },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::DERY },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::DERZ },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::DERW },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::INTX },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::INTY },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::INTZ },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::INTW },
        { Lexer::TokenType::COMMA, Lexer::TokenType::FLOAT },
        { Lexer::TokenType::COMMA, Lexer::TokenType::INT },
        { Lexer::TokenType::COMMA, Lexer::TokenType::ID },
        { Lexer::TokenType::COMMA, Lexer::TokenType::X },
        { Lexer::TokenType::COMMA, Lexer::TokenType::Y },
        { Lexer::TokenType::COMMA, Lexer::TokenType::Z },
        { Lexer::TokenType::COMMA, Lexer::TokenType::W },
        { Lexer::TokenType::COMMA, Lexer::TokenType::LPAR },
        { Lexer::TokenType::COMMA, Lexer::TokenType::MINUS },
        { Lexer::TokenType::COMMA, Lexer::TokenType::CALC },
        { Lexer::TokenType::COMMA, Lexer::TokenType::DERX },
        { Lexer::TokenType::COMMA, Lexer::TokenType::DERY },
        { Lexer::TokenType::COMMA, Lexer::TokenType::DERZ },
        { Lexer::TokenType::COMMA, Lexer::TokenType::DERW },
        { Lexer::TokenType::COMMA, Lexer::TokenType::INTX },
        { Lexer::TokenType::COMMA, Lexer::TokenType::INTY },
        { Lexer::TokenType::COMMA, Lexer::TokenType::INTZ },
        { Lexer::TokenType::COMMA, Lexer::TokenType::INTW },
        { Lexer::TokenType::CALC, Lexer::TokenType::LPAR },
        { Lexer::TokenType::DERX, Lexer::TokenType::LPAR },
        { Lexer::TokenType::DERY, Lexer::TokenType::LPAR },
        { Lexer::TokenType::DERZ, Lexer::TokenType::LPAR },
        { Lexer::TokenType::DERW, Lexer::TokenType::LPAR },
        { Lexer::TokenType::INTX, Lexer::TokenType::LPAR },
        { Lexer::TokenType::INTY, Lexer::TokenType::LPAR },
        { Lexer::TokenType::INTZ, Lexer::TokenType::LPAR },
        { Lexer::TokenType::INTW, Lexer::TokenType::LPAR }
    };

    bool validateToken(TokenType prev, TokenType cur)
    {
        return validTokenSequences.count({ prev, cur }) > 0;
    }

    bool isBinaryOperator(const Token& token)
    {
        switch (token.type())
        {
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::MULT:
        case TokenType::CARET:
        case TokenType::ASSIGN:
            return true;
        default:
            return false;
        }
    }

    bool isUnaryOperator(const Token& token)
    {
        switch (token.type())
        {
        case TokenType::MINUS:
            return true;
        default:
            return false;
        }
    }

    bool isFunction(const Token& token)
    {
        switch (token.type())
        {
        case TokenType::CALC:
        case TokenType::DERX:
        case TokenType::DERY:
        case TokenType::DERZ:
        case TokenType::DERW:
        case TokenType::INTX:
        case TokenType::INTY:
        case TokenType::INTZ:
        case TokenType::INTW:
            return true;
        default:
            return false;
        }
    }

    int getFuncArgCount(TokenType type)
    {
        switch (type)
        {
        case TokenType::CALC: return 4;
        case TokenType::DERX: return 1;
        case TokenType::DERY: return 1;
        case TokenType::DERZ: return 1;
        case TokenType::DERW: return 1;
        case TokenType::INTX: return 1;
        case TokenType::INTY: return 1;
        case TokenType::INTZ: return 1;
        case TokenType::INTW: return 1;
        default:
            throw std::invalid_argument(__FUNCTION__ ": Unknown operation provided");
        }
    }

    PostfixMember toMember(Token token, bool unary = false)
    {
        switch (token.type())
        {
        case TokenType::PLUS: return PostfixMember(token, 2);
        case TokenType::MINUS: return PostfixMember(token, 2, unary);
        case TokenType::MULT: return PostfixMember(token, 3);
        case TokenType::CARET: return PostfixMember(token, 4);
        case TokenType::CALC:
        case TokenType::DERX:
        case TokenType::DERY:
        case TokenType::DERZ:
        case TokenType::DERW:
        case TokenType::INTX:
        case TokenType::INTY:
        case TokenType::INTZ:
        case TokenType::INTW: return PostfixMember(token, 0, true);
        case TokenType::LPAR: return PostfixMember(token);
        default: throw std::invalid_argument(__FUNCTION__ ": Unknown operation provided");
        }
    }

    intr::op CompileOperation(const PostfixMember& member)
    {
        // check types and compile

        return (unsigned int)0;
    }

    std::variant<intr::program, SyntaxError> ExpressionCompiler::compileExpression(
        const std::vector<Lexer::Token>& tokens)
    {
        if (tokens.size() == 0 || tokens.back().type() != TokenType::ENDOFFILE)
            throw std::invalid_argument(__FUNCTION__ ": invalid token sequence");

        intr::program prog;
        Stack<PostfixMember> stack;
        Stack<int> argCounts;
        Stack<PostfixType> types;
        
        int parentheses = 0;
        size_t tokIndex = 0;
        Token prev = Token(TokenType::NONE);
        Token tok = prev;
        argCounts.Push(0);

        do
        {
            prev = tok;
            tok = tokens[tokIndex++];

            TokenType prevType = prev.type();
            TokenType curType = tok.type();

            if (!validateToken(prevType, curType))
                return SyntaxError{ tok.startPos(), "Unexpected token" };

            if (curType == TokenType::LPAR) parentheses++;
            else if (curType == TokenType::RPAR) parentheses--;
            if (parentheses < 0)
                return SyntaxError{ tok.startPos(), "Unexpected right parenthesis" };

            if (curType == TokenType::LPAR)
            {
                int argCount = 1;
                if (stack.Size() && isFunction(stack.Top().type()))
                    argCount = getFuncArgCount(stack.Top().type());

                argCounts.Push(argCount);
                stack.Push(toMember(tok));
            }
            else if (curType == TokenType::COMMA)
            {
                int argc = argCounts.Top() - 1;
                if (argc < 0) return SyntaxError{ tok.startPos(), "Too many arguments" };
                argCounts.Pop();
                argCounts.Push(argc);

                while (stack.Size() && stack.Top().type() != TokenType::LPAR)
                {
                    prog.push_back(CompileOperation(stack.Top()));
                    stack.Pop();
                }
            }
            else if (curType == TokenType::RPAR)
            {
                int argc = argCounts.Top() - 1;
                if (argc < 0) return SyntaxError{ tok.startPos(), "Too many arguments" };
                if (argc > 0) return SyntaxError{ tok.startPos(), "Not enough arguments" };

                while (stack.Top().type() != TokenType::LPAR)
                {
                    prog.push_back(CompileOperation(stack.Top()));
                    stack.Pop();
                }
                stack.Pop();

                while (stack.Size() && stack.Top().isPrefixOp())
                {
                    prog.push_back(CompileOperation(stack.Top()));
                    stack.Pop();
                }
            }
            else if (curType == TokenType::ENDOFFILE)
            {
                if (parentheses != 0) return SyntaxError{ tok.startPos(), "Unclosed parenthesis" };

                while (stack.Size())
                {
                    prog.push_back(CompileOperation(stack.Top()));
                    stack.Pop();
                }
            }
            else if (isUnaryOperator(tok) && (prevType == TokenType::NONE || prevType == TokenType::LPAR || isBinaryOperator(prev)))
            {
                stack.Push(toMember(tok, true));
            }
            else if (isFunction(tok))
            {
                stack.Push(toMember(tok));
            }
            else if (isBinaryOperator(tok))
            {
                PostfixMember member = toMember(tok);
                // todo: associativity
                while (stack.Size() && member.precedence() <= stack.Top().precedence())
                {
                    prog.push_back(CompileOperation(stack.Top()));
                    stack.Pop();
                }
                stack.Push(member);
            }
            else if (curType == TokenType::ID)
            {
                prog.push_back(CompileOperation(toMember(tok)));
                
                while (stack.Size() && stack.Top().isPrefixOp())
                {
                    prog.push_back(CompileOperation(stack.Top()));
                    stack.Pop();
                }
            }
            else
            {
                // TODO: Compile monomial
            }

        } while (tok.type() != TokenType::ENDOFFILE);

        return prog;
    }
}