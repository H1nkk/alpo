#pragma once
#include "linked_list.h"
#include "syntax_error.h"
#include <cstdint>
#include <string>
#include <variant>
#include <iostream>

class polynomial
{
private:
    class monomial
    {
    private:
        double mCoefficient;
        uint64_t mDegree;
    public:
        monomial() noexcept : mCoefficient(0.0), mDegree(0) {}
        monomial(double coefficient, uint32_t w, uint32_t x, uint32_t y, uint32_t z) noexcept : mCoefficient(coefficient),
            mDegree(((uint64_t)w << 48) | ((uint64_t)x << 32) | ((uint64_t)y << 16) | ((uint64_t)z))
        {
            if (w > UINT16_MAX || x > UINT16_MAX || y > UINT16_MAX || z > UINT16_MAX)
            {
                throw std::invalid_argument(__FUNCTION__ ": overflow occurred.");
            }
        }

        bool operator==(const monomial& other) const { return mCoefficient == other.mCoefficient && mDegree == other.mDegree; }
        bool operator!=(const monomial& other) const { return !operator==(other); }

        double coefficient() const noexcept { return mCoefficient; }
        void setCoefficient(double coefficient) { mCoefficient = coefficient; }
        uint64_t degree() const noexcept { return mDegree; }
        uint16_t w() const noexcept { return static_cast<uint16_t>(mDegree >> 48); }
        uint16_t x() const noexcept { return static_cast<uint16_t>(mDegree >> 32); }
        uint16_t y() const noexcept { return static_cast<uint16_t>(mDegree >> 16); }
        uint16_t z() const noexcept { return static_cast<uint16_t>(mDegree); }
    };

    linked_list<monomial> mMonomials;

    static std::variant<monomial, syntax_error> parse_monomial(const std::string& str, size_t& offset);
    static std::variant<polynomial, syntax_error> parse_polynomial(const std::string& str);

    explicit polynomial(const std::string& strRepr) {}
public:
    polynomial() {}

    static std::variant<polynomial, syntax_error> from_string(const std::string& str)
    {
        return parse_polynomial(str);
    }

    bool operator==(const polynomial& other) const;
    bool operator!=(const polynomial& other) const;

    polynomial operator+(const polynomial& other) const;
    polynomial& operator+=(const polynomial& other);
    polynomial operator-() const;
    polynomial operator-(const polynomial& other) const;
    polynomial& operator-=(const polynomial& other);
    polynomial operator*(const polynomial& other) const;
    polynomial operator*(double coefficient) const;
    friend polynomial operator*(double coefficient, const polynomial& p);
    polynomial& operator*=(double coefficient);
    friend std::ostream& operator<<(std::ostream& ostr, const polynomial& p);

    double evaluate(double w, double x, double y, double z) const;
    polynomial derivative_w() const;
    polynomial derivative_x() const;
    polynomial derivative_y() const;
    polynomial derivative_z() const;
    polynomial integral_x() const;
    polynomial integral_y() const;
    polynomial integral_z() const;
    polynomial integral_w() const;
};