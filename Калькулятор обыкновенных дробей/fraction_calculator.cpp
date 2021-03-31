#include <iostream>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <sstream> 
#include <set>
#include <map>
#include <vector>
using namespace std;

int gcd(int a, int b)
{
    return (b == 0) ? a : gcd(b, a % b);
}

class Rational {
public:
    Rational() {
        this->numerator = 0;
        this->denominator = 1;
    }

    Rational(int numerator, int denominator) {
        if (numerator == 0) {
            this->numerator = 0;
            this->denominator = 1;
            return;
        }
        if (denominator == 0) {
            throw std::invalid_argument("Invalid argument");
        }
        int nod = gcd(abs(numerator), abs(denominator));
        int sign = (numerator / abs(numerator)) / (denominator / abs(denominator));
        this->numerator = abs(numerator) * sign / nod;
        this->denominator = abs(denominator) / nod;
    }

    int Numerator() const {
        return this->numerator;
    }

    int Denominator() const {
        return this->denominator;
    }

    /*void SetDenominator(int denominator) {
        this->denominator = denominator;
    }

    void SetNumerator(int numerator) {
        this->numerator = numerator;
    }*/

private:
    int numerator;
    int denominator;
};

Rational operator+(const Rational& a, const Rational& b) {
    return Rational(a.Numerator() * b.Denominator() + b.Numerator() * a.Denominator(), a.Denominator() * b.Denominator());
}

Rational operator-(const Rational& a, const Rational& b) {
    return Rational(a.Numerator() * b.Denominator() - b.Numerator() * a.Denominator(), a.Denominator() * b.Denominator());
}

Rational operator*(const Rational& a, const Rational& b) {
    return Rational(a.Numerator() * b.Numerator(), a.Denominator() * b.Denominator());
}

Rational operator/(const Rational& a, const Rational& b) {
    if (b.Numerator() == 0) {
        throw domain_error("Division by zero");
    }
    return Rational(a.Numerator() * b.Denominator(), a.Denominator() * b.Numerator());
}

bool operator==(const Rational& a, const Rational& b) {
    return a.Numerator() == b.Numerator() && a.Denominator() == b.Denominator();
}

bool operator<(const Rational& a, const Rational& b) {
    return a.Numerator() * b.Denominator() < b.Numerator() * a.Denominator();
}

bool operator>(const Rational& a, const Rational& b) {
    return a.Numerator() * b.Denominator() > b.Numerator() * a.Denominator();
}

ostream& operator<<(ostream& stream, const Rational& rational) {
    stream << rational.Numerator() << '/' << rational.Denominator();
    return stream;
}

istream& operator>>(istream& stream, Rational& rational) {
    /*if (stream.rdbuf()->in_avail() == 0) {
        rational = Rational();
        return stream;
    }*/
    int numerator, denominator;
    char sign;
    bool empty = !(stream >> numerator >> sign >> denominator);
    if (!empty && sign == '/') {
        rational = Rational(numerator, denominator);
    }
    return stream;
}

int main() {
    try
    {
        Rational x, y;
        char sign;
        cin >> x;
        cin.ignore(1);
        cin >> sign;
        cin.ignore(1);
        cin >> y;
        switch (sign)
        {
        case '+':
            cout << x + y;
            break;
        case '-':
            cout << x - y;
            break;
        case '/':
            cout << x / y;
            break;
        case '*':
            cout << x * y;
            break;
        default:
            break;
        }
    }
    catch (const std::exception& error)
    {
        cout << error.what();
    }
    return 0;
}