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

TEST(PolynomialTest, can_multiply_polynomials)
{
    polynomial p1 = std::get<polynomial>(polynomial::from_string("2w^2 + 3x"));
    polynomial p2 = std::get<polynomial>(polynomial::from_string("w^2 + x + 4"));

    polynomial prod = p1 * p2;

    EXPECT_EQ(prod, std::get<polynomial>(polynomial::from_string("2w4 + 5xw2 + 8w2 + 3x2 + 12x")));
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

TEST(PolynomialTest, can_get_w_integral)
{
    polynomial p = std::get<polynomial>(polynomial::from_string("3w^2 + 2x + z + 4y^3"));
    polynomial derivative = p.integral_w();

    EXPECT_EQ(derivative, std::get<polynomial>(polynomial::from_string("w^3 + 2xw + zw + 4wy^3")));
}

TEST(PolynomialTest, can_get_x_integral)
{
    polynomial p = std::get<polynomial>(polynomial::from_string("3w^2 + 2x + z + 4y^3"));
    polynomial derivative = p.integral_x();

    EXPECT_EQ(derivative, std::get<polynomial>(polynomial::from_string("3xw^2 + x^2 + zx + 4xy^3")));
}

TEST(PolynomialTest, can_get_y_integral)
{
    polynomial p = std::get<polynomial>(polynomial::from_string("3w^2 + 2x + z + 4y^3"));
    polynomial derivative = p.integral_y();

    EXPECT_EQ(derivative, std::get<polynomial>(polynomial::from_string("3yw^2 + 2xy + zy + y^4")));
}

TEST(PolynomialTest, can_get_z_integral)
{
    polynomial p = std::get<polynomial>(polynomial::from_string("3w^2 + 2x + z + 4y^3"));
    polynomial derivative = p.integral_z();

    EXPECT_EQ(derivative, std::get<polynomial>(polynomial::from_string("3zw^2 + 2xz + 0.5z^2 + 4zy^3")));
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
    auto result = polynomial::from_string("2w^65536");
    EXPECT_TRUE(std::holds_alternative<syntax_error>(result));
    if (std::holds_alternative<syntax_error>(result))
    {
        syntax_error err = std::get<syntax_error>(result);
        EXPECT_EQ(err.message, "Too big power! Maximum supported power is 65535");
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

TEST(PolynomialTest, error_overflow_on_multiply)
{
    polynomial p1 = std::get<polynomial>(polynomial::from_string("2w^65000"));
    polynomial p2 = std::get<polynomial>(polynomial::from_string("w^65000"));

    EXPECT_ANY_THROW(p1 * p2);
}

TEST(PolynomialTest, error_overflow_on_int_w)
{
    polynomial p1 = std::get<polynomial>(polynomial::from_string("2w^65535"));
    EXPECT_ANY_THROW(p1.integral_w());
}

TEST(PolynomialTest, error_overflow_on_int_x)
{
    polynomial p1 = std::get<polynomial>(polynomial::from_string("2x^65535"));
    EXPECT_ANY_THROW(p1.integral_x());
}

TEST(PolynomialTest, error_overflow_on_int_y)
{
    polynomial p1 = std::get<polynomial>(polynomial::from_string("2y^65535"));
    EXPECT_ANY_THROW(p1.integral_y());
}

TEST(PolynomialTest, error_overflow_on_int_z)
{
    polynomial p1 = std::get<polynomial>(polynomial::from_string("2z^65535"));
    EXPECT_ANY_THROW(p1.integral_z());
}

TEST(PolynomialTest, error_on_invalid_coefficient)
{
    auto result = polynomial::from_string("3.5.2x");
    EXPECT_TRUE(std::holds_alternative<syntax_error>(result));
    if (std::holds_alternative<syntax_error>(result))
    {
        syntax_error err = std::get<syntax_error>(result);
        EXPECT_EQ(err.message, "Invalid coefficient!");
    }
}

TEST(PolynomialTest, error_on_repeated_variable)
{
    auto result = polynomial::from_string("2xx");
    EXPECT_TRUE(std::holds_alternative<syntax_error>(result));
    if (std::holds_alternative<syntax_error>(result))
    {
        syntax_error err = std::get<syntax_error>(result);
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