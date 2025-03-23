#include <gtest/gtest.h>
#include "lexer/lexer.h"
#include "expression_compiler/expression_compiler.h"
#include <vector>
#include <variant>

TEST(ExprCompilerTest, can_compile_single_int)
{
    Compiler::ExpressionCompiler c;
    auto res = c.compileExpression(Lexer::Lexer("12331").getAllTokens());
    
    EXPECT_TRUE(std::holds_alternative<intr::program>(res));
    
    intr::program p = std::get<intr::program>(res);
    EXPECT_EQ(p.size(), 1);
    EXPECT_TRUE(std::holds_alternative<unsigned long>(p[0]));
    EXPECT_EQ(std::get<unsigned long>(p[0]), 12331);
}

TEST(ExprCompilerTest, error_on_big_integer)
{
    Compiler::ExpressionCompiler c;
    auto res = c.compileExpression(Lexer::Lexer("123313249873429874329213").getAllTokens());

    EXPECT_TRUE(std::holds_alternative<SyntaxError>(res));
}

TEST(ExprCompilerTest, can_compile_single_float)
{
    Compiler::ExpressionCompiler c;
    auto res = c.compileExpression(Lexer::Lexer("123.31").getAllTokens());

    EXPECT_TRUE(std::holds_alternative<intr::program>(res));

    intr::program p = std::get<intr::program>(res);
    EXPECT_EQ(p.size(), 1);
    EXPECT_TRUE(std::holds_alternative<double>(p[0]));
    EXPECT_EQ(std::get<double>(p[0]), 123.31);
}

TEST(ExprCompilerTest, error_on_big_float)
{
    Compiler::ExpressionCompiler c;
    auto res = c.compileExpression(Lexer::Lexer(std::string(400, '1') + ".2133213").getAllTokens());

    EXPECT_TRUE(std::holds_alternative<SyntaxError>(res));
}

TEST(ExprCompilerTest, can_compile_identifier) 
{
    Compiler::ExpressionCompiler c;
    auto res = c.compileExpression(Lexer::Lexer("myVeryGoodPoly").getAllTokens());

    EXPECT_TRUE(std::holds_alternative<intr::program>(res));

    intr::program p = std::get<intr::program>(res);
    EXPECT_EQ(p.size(), 1);
    EXPECT_TRUE(std::holds_alternative<std::string>(p[0]));
    EXPECT_EQ(std::get<std::string>(p[0]), "myVeryGoodPoly");
}

TEST(ExprCompilerTest, can_compile_single_monomial)
{
    Compiler::ExpressionCompiler c;
    auto res = c.compileExpression(Lexer::Lexer("12.12x^39yz12w1 ").getAllTokens());

    EXPECT_TRUE(std::holds_alternative<intr::program>(res));

    intr::program p = std::get<intr::program>(res);
    EXPECT_EQ(p.size(), 1);
    EXPECT_TRUE(std::holds_alternative<polynomial>(p[0]));
    EXPECT_EQ(std::get<polynomial>(p[0]), std::get<polynomial>(polynomial::from_string("12.12x^39yz12w1")));
}

TEST(ExprCompilerTest, can_compile_sum)
{
    Compiler::ExpressionCompiler c;
    auto res = c.compileExpression(Lexer::Lexer("12.12x^39yz12 + 120 + pol").getAllTokens());

    EXPECT_TRUE(std::holds_alternative<intr::program>(res));

    intr::program p = std::get<intr::program>(res);
    EXPECT_EQ(p.size(), 5);
    EXPECT_TRUE(std::holds_alternative<polynomial>(p[0]));
    EXPECT_EQ(std::get<polynomial>(p[0]), std::get<polynomial>(polynomial::from_string("12.12x^39yz12")));
    EXPECT_TRUE(std::holds_alternative<unsigned long>(p[1]));
    EXPECT_EQ(std::get<unsigned long>(p[1]), 120);
    EXPECT_TRUE(std::holds_alternative<intr::opcode>(p[2]));
    EXPECT_EQ(std::get<intr::opcode>(p[2]), intr::opcode::ADD);
    EXPECT_TRUE(std::holds_alternative<std::string>(p[3]));
    EXPECT_EQ(std::get<std::string>(p[3]), "pol");
    EXPECT_TRUE(std::holds_alternative<intr::opcode>(p[4]));
    EXPECT_EQ(std::get<intr::opcode>(p[4]), intr::opcode::ADD);
}

TEST(ExprCompilerTest, can_compile_subtraction)
{
    Compiler::ExpressionCompiler c;
    auto res = c.compileExpression(Lexer::Lexer("y - 12.12x^39 - 120").getAllTokens());

    EXPECT_TRUE(std::holds_alternative<intr::program>(res));

    intr::program p = std::get<intr::program>(res);
    EXPECT_EQ(p.size(), 5);
    EXPECT_TRUE(std::holds_alternative<polynomial>(p[0]));
    EXPECT_EQ(std::get<polynomial>(p[0]), std::get<polynomial>(polynomial::from_string("y")));
    EXPECT_TRUE(std::holds_alternative<polynomial>(p[1]));
    EXPECT_EQ(std::get<polynomial>(p[1]), std::get<polynomial>(polynomial::from_string("12.12x^39")));
    EXPECT_TRUE(std::holds_alternative<intr::opcode>(p[2]));
    EXPECT_EQ(std::get<intr::opcode>(p[2]), intr::opcode::SUBTRACT);
    EXPECT_TRUE(std::holds_alternative<unsigned long>(p[3]));
    EXPECT_EQ(std::get<unsigned long>(p[3]), 120);
    EXPECT_TRUE(std::holds_alternative<intr::opcode>(p[4]));
    EXPECT_EQ(std::get<intr::opcode>(p[4]), intr::opcode::SUBTRACT);
}

TEST(ExprCompilerTest, can_compile_unary_minus)
{
    Compiler::ExpressionCompiler c;
    auto res = c.compileExpression(Lexer::Lexer("-120 + y").getAllTokens());

    EXPECT_TRUE(std::holds_alternative<intr::program>(res));

    intr::program p = std::get<intr::program>(res);
    EXPECT_EQ(p.size(), 4);
    EXPECT_TRUE(std::holds_alternative<unsigned long>(p[0]));
    EXPECT_EQ(std::get<unsigned long>(p[0]), 120);
    EXPECT_TRUE(std::holds_alternative<intr::opcode>(p[1]));
    EXPECT_EQ(std::get<intr::opcode>(p[1]), intr::opcode::UMINUS);
    EXPECT_TRUE(std::holds_alternative<polynomial>(p[2]));
    EXPECT_EQ(std::get<polynomial>(p[2]), std::get<polynomial>(polynomial::from_string("y")));
    EXPECT_TRUE(std::holds_alternative<intr::opcode>(p[3]));
    EXPECT_EQ(std::get<intr::opcode>(p[3]), intr::opcode::ADD);
}

TEST(ExprCompilerTest, can_compile_multiplication)
{
    Compiler::ExpressionCompiler c;
    auto res = c.compileExpression(Lexer::Lexer("y * pol").getAllTokens());

    EXPECT_TRUE(std::holds_alternative<intr::program>(res));

    intr::program p = std::get<intr::program>(res);
    EXPECT_EQ(p.size(), 3);
    EXPECT_TRUE(std::holds_alternative<polynomial>(p[0]));
    EXPECT_EQ(std::get<polynomial>(p[0]), std::get<polynomial>(polynomial::from_string("y")));
    EXPECT_TRUE(std::holds_alternative<std::string>(p[1]));
    EXPECT_EQ(std::get<std::string>(p[1]), "pol");
    EXPECT_TRUE(std::holds_alternative<intr::opcode>(p[2]));
    EXPECT_EQ(std::get<intr::opcode>(p[2]), intr::opcode::MULT);
}

TEST(ExprCompilerTest, can_compile_calc)
{
    Compiler::ExpressionCompiler c;
    auto res = c.compileExpression(Lexer::Lexer("calc(2xyzw, 10.3, 100, -20.3, 1.02)").getAllTokens());

    EXPECT_TRUE(std::holds_alternative<intr::program>(res));

    intr::program p = std::get<intr::program>(res);
    EXPECT_EQ(p.size(), 7);
    EXPECT_TRUE(std::holds_alternative<polynomial>(p[0]));
    EXPECT_EQ(std::get<polynomial>(p[0]), std::get<polynomial>(polynomial::from_string("2xyzw")));
    EXPECT_TRUE(std::holds_alternative<double>(p[1]));
    EXPECT_EQ(std::get<double>(p[1]), 10.3);
    EXPECT_TRUE(std::holds_alternative<unsigned long>(p[2]));
    EXPECT_EQ(std::get<unsigned long>(p[2]), 100);
    EXPECT_TRUE(std::holds_alternative<double>(p[3]));
    EXPECT_EQ(std::get<double>(p[3]), 20.3);
    EXPECT_TRUE(std::holds_alternative<intr::opcode>(p[4]));
    EXPECT_EQ(std::get<intr::opcode>(p[4]), intr::opcode::UMINUS);
    EXPECT_TRUE(std::holds_alternative<double>(p[5]));
    EXPECT_EQ(std::get<double>(p[5]), 1.02);
    EXPECT_TRUE(std::holds_alternative<intr::opcode>(p[6]));
    EXPECT_EQ(std::get<intr::opcode>(p[6]), intr::opcode::CALC);
}

TEST(ExprCompilerTest, can_compile_derivative)
{
    ADD_FAILURE();
}

TEST(ExprCompilerTest, can_compile_integral)
{
    ADD_FAILURE();
}

TEST(ExprCompilerTest, can_compile_assignment)
{
    ADD_FAILURE();
}

TEST(ExprCompilerTest, can_compile_expression_with_parenthesis)
{
    ADD_FAILURE();
}

TEST(ExprCompilerTest, can_handle_operator_precedence)
{
    Compiler::ExpressionCompiler c;
    auto res = c.compileExpression(Lexer::Lexer("12 + y * pol").getAllTokens());

    EXPECT_TRUE(std::holds_alternative<intr::program>(res));

    intr::program p = std::get<intr::program>(res);
    EXPECT_EQ(p.size(), 5);
    EXPECT_TRUE(std::holds_alternative<unsigned long>(p[0]));
    EXPECT_EQ(std::get<unsigned long>(p[0]), 12);
    EXPECT_TRUE(std::holds_alternative<polynomial>(p[1]));
    EXPECT_EQ(std::get<polynomial>(p[1]), std::get<polynomial>(polynomial::from_string("y")));
    EXPECT_TRUE(std::holds_alternative<std::string>(p[2]));
    EXPECT_EQ(std::get<std::string>(p[2]), "pol");
    EXPECT_TRUE(std::holds_alternative<intr::opcode>(p[3]));
    EXPECT_EQ(std::get<intr::opcode>(p[3]), intr::opcode::MULT);
    EXPECT_TRUE(std::holds_alternative<intr::opcode>(p[4]));
    EXPECT_EQ(std::get<intr::opcode>(p[4]), intr::opcode::ADD);
}

TEST(ExprCompilerTest, can_perform_complex_calculation)
{
    ADD_FAILURE();
}

TEST(ExprCompilerTest, exception_on_unclosed_parenthesis)
{
    ADD_FAILURE();
}

TEST(ExprCompilerTest, exception_on_too_many_parentheses)
{
    ADD_FAILURE();
}

TEST(ExprCompilerTest, exception_on_empty_parentheses)
{
    ADD_FAILURE();
}

TEST(ExprCompilerTest, exception_on_two_operators_in_a_row)
{
    ADD_FAILURE();
}

TEST(ExprCompilerTest, exception_on_binary_operator_without_left_argument)
{
    ADD_FAILURE();
}

TEST(ExprCompilerTest, exception_on_binary_operator_without_right_argument)
{
    ADD_FAILURE();
}

TEST(ExprCompilerTest, exception_on_function_use_without_parentheses)
{
    ADD_FAILURE();
}

TEST(ExprCompilerTest, exception_on_function_too_many_params)
{
    ADD_FAILURE();
}

TEST(ExprCompilerTest, exception_on_function_not_enough_params)
{
    ADD_FAILURE();
}