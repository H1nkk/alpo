#include "polynomial.h"
#include <cmath>
#include <algorithm>
#include <map>
#include <vector>

polynomial polynomial::operator+(const polynomial& other) const
{
    polynomial result;

    auto it1 = mMonomials.begin();
    auto it2 = other.mMonomials.begin();

    while (it1 != mMonomials.end() && it2 != other.mMonomials.end())
    {
        uint32_t deg1 = (*it1).degree();
        uint32_t deg2 = (*it2).degree();

        if (deg1 > deg2)
        {
            result.mMonomials.push_back(*it1);
            ++it1;
        } else if (deg2 > deg1)
        {
            result.mMonomials.push_back(*it2);
            ++it2;
        } else
        {
            double coefficient = (*it1).coefficient() + (*it2).coefficient();
            if (coefficient != 0.0)
            {
                result.mMonomials.push_back(monomial(coefficient, (*it1).w(), (*it1).x(), (*it1).y(), (*it1).z()));
            }
            ++it1;
            ++it2;
        }
    }

    while (it1 != mMonomials.end())
    {
        result.mMonomials.push_back(*it1);
        ++it1;
    }

    while (it2 != other.mMonomials.end())
    {
        result.mMonomials.push_back(*it2);
        ++it2;
    }

    return result;
}

polynomial polynomial::operator*(double coefficient) const
{
    polynomial result;

    if (coefficient == 0.0) return result;

    for (auto it = mMonomials.begin(); it != mMonomials.end(); ++it)
    {
        result.mMonomials.push_back(monomial(coefficient * (*it).coefficient(), (*it).w(), (*it).x(), (*it).y(), (*it).z()));
    }

    return result;
}

polynomial operator*(double coefficient, const polynomial& p)
{
    return p * coefficient;
}

std::ostream& operator<<(std::ostream& ostr, const polynomial& p)
{
    if (p.mMonomials.size() == 0)
    {
        ostr << 0;
        return ostr;
    }

    bool first = true;
    for (auto& v : p.mMonomials)
    {
        if (!first && v.coefficient() > 0.0) ostr << '+';
        first = false;

        ostr << v.coefficient();
        if (v.w())
        {
            ostr << "*w";
            if (v.w() > 1) ostr << '^' << static_cast<int32_t>(v.w());
        }
        if (v.x())
        {
            ostr << "*x";
            if (v.x() > 1) ostr << '^' << static_cast<int32_t>(v.x());
        }
        if (v.y())
        {
            ostr << "*y";
            if (v.y() > 1) ostr << '^' << static_cast<int32_t>(v.y());
        }
        if (v.z())
        {
            ostr << "*z";
            if (v.z() > 1) ostr << '^' << static_cast<int32_t>(v.z());
        }
    }

    return ostr;
}

polynomial polynomial::operator-() const
{
    return (*this) * -1.0;
}

polynomial polynomial::operator-(const polynomial& other) const
{
    return (*this) + (-other);
}

polynomial& polynomial::operator+=(const polynomial& other)
{
    *this = *this + other;
    return *this;
}

polynomial& polynomial::operator-=(const polynomial& other)
{
    *this = *this - other;
    return *this;
}

polynomial& polynomial::operator*=(double coefficient)
{
    *this = *this * coefficient;
    return *this;
}

bool polynomial::operator==(const polynomial& other) const
{
    if (mMonomials.size() != other.mMonomials.size())
    {
        return false;
    }

    for (auto it1 = mMonomials.begin(), it2 = other.mMonomials.begin(); it1 != mMonomials.end() && it2 != other.mMonomials.end(); ++it1, ++it2)
    {
        if (*it1 != *it2)
        {
            return false;
        }
    }

    return true;
}

bool polynomial::operator!=(const polynomial& other) const
{
    return !operator==(other);
}

double polynomial::evaluate(double w, double x, double y, double z) const
{
    double res = 0.0;

    for (auto it = mMonomials.begin(); it != mMonomials.end(); ++it)
    {
        res += (*it).coefficient() *
            pow(w, static_cast<double>((*it).w())) *
            pow(x, static_cast<double>((*it).x())) *
            pow(y, static_cast<double>((*it).y())) *
            pow(z, static_cast<double>((*it).z()));
    }

    return res;
}

polynomial polynomial::derivative_w() const
{
    polynomial res;

    for (auto it = mMonomials.begin(); it != mMonomials.end(); ++it)
    {
        uint8_t degree = (*it).w();
        if (degree > 0)
        {
            res.mMonomials.push_back(
                monomial(static_cast<double>(degree) * (*it).coefficient(), (*it).w() - 1, (*it).x(), (*it).y(), (*it).z())
            );
        }
    }

    return res;
}

polynomial polynomial::derivative_x() const
{
    polynomial res;

    for (auto it = mMonomials.begin(); it != mMonomials.end(); ++it)
    {
        uint8_t degree = (*it).x();
        if (degree > 0)
        {
            res.mMonomials.push_back(
                monomial(static_cast<double>(degree) * (*it).coefficient(), (*it).w(), (*it).x() - 1, (*it).y(), (*it).z())
            );
        }
    }

    return res;
}

polynomial polynomial::derivative_y() const
{
    polynomial res;

    for (auto it = mMonomials.begin(); it != mMonomials.end(); ++it)
    {
        uint8_t degree = (*it).y();
        if (degree > 0)
        {
            res.mMonomials.push_back(
                monomial(static_cast<double>(degree) * (*it).coefficient(), (*it).w(), (*it).x(), (*it).y() - 1, (*it).z())
            );
        }
    }

    return res;
}

polynomial polynomial::derivative_z() const
{
    polynomial res;

    for (auto it = mMonomials.begin(); it != mMonomials.end(); ++it)
    {
        uint8_t degree = (*it).z();
        if (degree > 0)
        {
            res.mMonomials.push_back(
                monomial(static_cast<double>(degree) * (*it).coefficient(), (*it).w(), (*it).x(), (*it).y(), (*it).z() - 1)
            );
        }
    }

    return res;
}

std::variant<polynomial::monomial, SyntaxError> polynomial::parse_monomial(const std::string& str, size_t& offset)
{
    std::string valid = "+-.1234567890wxyz";
    if (valid.find(str[offset]) == std::string::npos)
    {
        return SyntaxError{ offset, "Unexpected symbol" };
    }

    std::string coefficient;
    if (str[offset] == '+') ++offset;
    while (offset < str.size() && (str[offset] == '-' || str[offset] == '.' || (str[offset] >= '0' && str[offset] <= '9')))
    {
        coefficient.push_back(str[offset]);
        ++offset;
    }

    double coefd;
    if (coefficient == "-") coefd = -1.0;
    else if (coefficient.size() == 0) coefd = 1.0;
    else
    {
        size_t coefl = 0;
        coefd = std::stod(coefficient, &coefl);
        if (coefl != coefficient.size()) return SyntaxError{ offset, "Invalid coefficient!" };
    }

    uint8_t powers[4]{};

    while (offset < str.size() && (str[offset] == 'w' || str[offset] == 'x' || str[offset] == 'y' || str[offset] == 'z'))
    {
        uint8_t& p = powers[str[offset] - 'w'];
        if (p != 0)
        {
            return SyntaxError{ offset,"Variables in monomes should be mentioned no more than once." };
        }

        p = 1;
        ++offset;

        if (offset == str.size()) break;
        if (str[offset] == '^')
        {
            ++offset;
            if (offset == str.size()) break;
        }

        std::string power;
        while (offset < str.size() && (str[offset] >= '0' && str[offset] <= '9'))
        {
            power.push_back(str[offset++]);
        }

        if (power.size())
        {
            int32_t poweri = std::stoi(power);
            if (poweri > 255) return SyntaxError{ offset, "Too big power! Maximum supported power is 255" };
            p = poweri;
        }
    }

    return monomial(coefd, powers[0], powers[1], powers[2], powers[3]);
}

std::variant<polynomial, SyntaxError> polynomial::parse_polynomial(const std::string& str)
{
    std::string nospaces;
    std::vector<size_t> originalIndices;
    
    for (size_t i = 0; i < str.size(); ++i)
    {
        if (!isspace(str[i]))
        {
            nospaces.push_back(str[i]);
            originalIndices.push_back(i);
        }
    }
    originalIndices.push_back(str.size());

    std::map<uint32_t, monomial> monomials;
    size_t offset = 0;
    while (offset < nospaces.size())
    {
        auto res = parse_monomial(nospaces, offset);
        if (res.index())
        {
            SyntaxError err = std::get<SyntaxError>(res);
            err.pos = originalIndices[err.pos];
            return err;
        }
        monomial m = std::get<monomial>(res);

        if (m.coefficient() == 0.0) continue;

        if (monomials.count(m.degree()))
        {
            return SyntaxError{ originalIndices[offset], "A polynomial must contain no more than one monomial of each degree." };
        }

        monomials[m.degree()] = m;
    }

    polynomial p;
    for (auto it = monomials.rbegin(); it != monomials.rend(); ++it)
    {
        p.mMonomials.push_back(it->second);
    }

    return p;
}