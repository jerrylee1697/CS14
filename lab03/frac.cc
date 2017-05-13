#include <cmath>
#include <iostream>
using namespace std;

class Fraction
{
        int gcd(int a, int b) { return b==0 ? a : gcd(b,a%b); }
        int n, d;
    public:
        Fraction(int n, int d = 1) : n(n/gcd(std::abs(n),std::abs(d))), d(d/gcd(std::abs(n),std::abs(d))) {
            if (this->d < 0) {
                this->n *= -1;
                this->d *= -1;
            }
        }
        int num() const { return n; }
        int den() const { return d; }
        Fraction& operator*=(const Fraction& rhs) {
            int new_n = n*rhs.n / gcd(std::abs(n*rhs.n), std::abs(d*rhs.d));
            d = d*rhs.d / gcd(std::abs(n*rhs.n), std::abs(d*rhs.d));
            n = new_n;
            return *this;
        }
        Fraction& operator-=(const Fraction& rhs) {
            int new_n = n*rhs.d - d*rhs.n / gcd(std::abs(n*rhs.n), std::abs(d*rhs.d));
            d = d*rhs.d / gcd(std::abs(n*rhs.n), std::abs(d*rhs.d));
            n = new_n;
            return *this;
        }
};

std::ostream& operator<<(std::ostream& out, const Fraction& f) {
    return out << f.num() << '/' << f.den() ;
}
bool operator==(const Fraction& lhs, const Fraction& rhs) {
    return ((lhs.num() * rhs.den()) == (rhs.num() * lhs.den()));
}
bool operator<(const Fraction& lhs, const Fraction& rhs) {
    return (lhs.num() * rhs.den() < rhs.num() * lhs.den());
}
bool operator>(const Fraction& lhs, const Fraction& rhs) {
    return (lhs.num() * rhs.den() > rhs.num() * lhs.den());
}
Fraction operator*(Fraction lhs, const Fraction& rhs) {
    return lhs *= rhs;
}
Fraction operator-(Fraction lhs, const Fraction& rhs) {
    return lhs -= rhs;
}