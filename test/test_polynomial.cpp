#include <gtest/gtest.h>
#include "polynomial.h"

TEST(PolynomialTest, can_create_with_defailt_constructor)
{
    polynomial p;
    EXPECT_EQ(p.evaluate(1, 1, 1, 1), 0.0);
}

TEST(PolynomialTest, can_add_polynomials)
{
    polynomial p1 = std::get<polynomial>(polynomial::from_string("2w^2 + 3x"));
    polynomial p2 = std::get<polynomial>(polynomial::from_string("w^2 + x + 4"));

    polynomial sum = p1 + p2;

    EXPECT_EQ(sum, std::get<polynomial>(polynomial::from_string("3w^2 + 4x + 4")));
}

TEST(PolynomialTest, can_subtract_polynomials)
{
    polynomial p1 = std::get<polynomial>(polynomial::from_string("2w^2 + 3x"));
    polynomial p2 = std::get<polynomial>(polynomial::from_string("w^2 + x + 4"));

    polynomial sum = p1 - p2;

    EXPECT_EQ(sum, std::get<polynomial>(polynomial::from_string("w^2 + 2x - 4")));
}

TEST(PolynomialTest, can_negate_polynomial)
{
    polynomial p1 = std::get<polynomial>(polynomial::from_string("2w^2 + 3x"));
    polynomial p2 = std::get<polynomial>(polynomial::from_string("-2w^2 - 3x"));

    EXPECT_EQ(p1, -p2);
}

TEST(PolynomialTest, can_multiply_by_scalar)
{
    polynomial p = std::get<polynomial>(polynomial::from_string("2w^2 + 3x"));
    polynomial result = p * 2.0;

    EXPECT_EQ(result, std::get<polynomial>(polynomial::from_string("4w^2 + 6x")));
}

TEST(PolynomialTest, can_get_w_derivative)
{
    polynomial p = std::get<polynomial>(polynomial::from_string("3w^2 + 2x + z"));
    polynomial derivative = p.derivative_w();

    EXPECT_EQ(derivative, std::get<polynomial>(polynomial::from_string("6w")));
}

TEST(PolynomialTest, can_get_x_derivative)
{
    polynomial p = std::get<polynomial>(polynomial::from_string("3w^2 + 2x + z"));
    polynomial derivative = p.derivative_x();

    EXPECT_EQ(derivative, std::get<polynomial>(polynomial::from_string("2")));
}

TEST(PolynomialTest, can_get_y_derivative)
{
    polynomial p = std::get<polynomial>(polynomial::from_string("3w^2 + 2x + z + 4y^3"));
    polynomial derivative = p.derivative_y();

    EXPECT_EQ(derivative, std::get<polynomial>(polynomial::from_string("12y^2")));
}

TEST(PolynomialTest, can_get_z_derivative)
{
    polynomial p = std::get<polynomial>(polynomial::from_string("3w^2 + 2x + z + 4y^3"));
    polynomial derivative = p.derivative_z();

    EXPECT_EQ(derivative, std::get<polynomial>(polynomial::from_string("1")));
}

TEST(PolynomialTest, can_compare)
{
    polynomial p1 = std::get<polynomial>(polynomial::from_string("2w^2 + 3x"));
    polynomial p2 = std::get<polynomial>(polynomial::from_string("2w^2 + 3x"));
    polynomial p3 = std::get<polynomial>(polynomial::from_string("w^2 + 3x"));

    EXPECT_TRUE(p1 == p2);
    EXPECT_TRUE(p1 != p3);
}

TEST(PolynomialTest, check_power_limit)
{
    auto result = polynomial::from_string("2w^256");
    EXPECT_TRUE(std::holds_alternative<SyntaxError>(result));
    if (std::holds_alternative<SyntaxError>(result))
    {
        SyntaxError err = std::get<SyntaxError>(result);
        EXPECT_EQ(err.message, "Too big power! Maximum supported power is 255");
    }
}

TEST(PolynomialTest, can_parse_with_spaces)
{
    auto result = polynomial::from_string("  2 w ^ 2  + 3 x  + 4  ");
    EXPECT_TRUE(std::holds_alternative<polynomial>(result));
    if (std::holds_alternative<polynomial>(result))
    {
        polynomial p = std::get<polynomial>(result);
        EXPECT_EQ(p.evaluate(2, 1, 1, 1), 15.0);
    }
}

TEST(PolynomialTest, test_ostream)
{
    polynomial p = std::get<polynomial>(polynomial::from_string("2w^2 + 3x + 4"));
    std::ostringstream oss;
    oss << p;
    EXPECT_EQ(oss.str(), "2*w^2+3*x+4");
}

TEST(PolynomialTest, multiplying_by_zero_gives_zero_polynomial)
{
    polynomial p = std::get<polynomial>(polynomial::from_string("3w^2 + 2x + z"));
    polynomial result = p * 0.0;
    EXPECT_EQ(result, polynomial());
}

TEST(PolynomialTest, can_add_zero_polynomial)
{
    polynomial p1 = std::get<polynomial>(polynomial::from_string("3w^2 + 2x + z"));
    polynomial zeroPoly;
    polynomial result = p1 + zeroPoly;
    EXPECT_EQ(result, p1);
}

TEST(PolynomialTest, returns_empty_derivative)
{
    polynomial p = std::get<polynomial>(polynomial::from_string("3w^2 + 2x + z"));
    polynomial derivative = p.derivative_y();
    EXPECT_EQ(derivative, polynomial());
}

TEST(PolynomialTest, error_on_invalid_coefficient)
{
    auto result = polynomial::from_string("3.5.2x");
    EXPECT_TRUE(std::holds_alternative<SyntaxError>(result));
    if (std::holds_alternative<SyntaxError>(result))
    {
        SyntaxError err = std::get<SyntaxError>(result);
        EXPECT_EQ(err.message, "Invalid coefficient!");
    }
}

TEST(PolynomialTest, error_on_repeated_variable)
{
    auto result = polynomial::from_string("2xx");
    EXPECT_TRUE(std::holds_alternative<SyntaxError>(result));
    if (std::holds_alternative<SyntaxError>(result))
    {
        SyntaxError err = std::get<SyntaxError>(result);
        EXPECT_EQ(err.message, "Variables in monomes should be mentioned no more than once.");
    }
}

TEST(PolynomialTest, empty_string_gives_zero_polynomial)
{
    auto result = polynomial::from_string("");
    EXPECT_TRUE(std::holds_alternative<polynomial>(result));
    if (std::holds_alternative<polynomial>(result))
    {
        polynomial p = std::get<polynomial>(result);
        EXPECT_EQ(p, polynomial());
    }
}

TEST(PolynomialTest, self_subtraction_gives_zero)
{
    polynomial p1 = std::get<polynomial>(polynomial::from_string("w^2 + 2x"));
    polynomial result = p1 - p1;
    EXPECT_EQ(result, polynomial());
}

TEST(PolynomialTest, empty_polynomial_displayed_as_zero)
{
    polynomial p;
    std::ostringstream oss;
    oss << p;
    EXPECT_EQ(oss.str(), "0");
}

TEST(PolynomialTest, can_evaluate_polynomial_correctly)
{
    auto result = polynomial::from_string("3.5w^2 - x^2 + 4y^3 - 2.5z");
    EXPECT_TRUE(std::holds_alternative<polynomial>(result));
    if (std::holds_alternative<polynomial>(result))
    {
        polynomial p = std::get<polynomial>(result);
        EXPECT_DOUBLE_EQ(p.evaluate(1, 2, 3, 4), 3.5 * 1.0 * 1.0 - 2.0 * 2.0 + 4.0 * 3.0 * 3.0 * 3.0 - 2.5 * 4.0);
    }
}