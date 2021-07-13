#include "rational.h"

    rational::rational(int a, int b) {
        q[0] = abs(a) * sgn(a * b);
        q[1] = abs(b);
    }

    void rational::simplify() {
        int div = gcd(q[0], q[1]);
        q[0] = q[0] / div;
        q[1] = q[1] / div;
    }

    //Access elements

    int& rational::operator [](int i) {
        if (i > 2 || i < 0) {
            exit(5);
        }
        else
            return q[i];
    }

    //Assignment

    rational rational::operator =(rational q2) {
        q[0] = q2[0];
        q[1] = q2[1];
        this->simplify();
        return *this;
    }

    rational rational::operator =(int n) {
        q[0] = n;
        q[1] = 1;
        this->simplify();
        return *this;
    }

    rational rational::operator =(const char *s) {
        sscanf_s(s, "%d/%d", &q[0], &q[1]);
        return *this;
    }

    //Basic Arithmetic

    rational rational::operator +(rational q2) {
        int den = q[1] * q2[1];
        int num = (q[0] * q2[1] + q[1] * q2[0]);
        rational ans(num, den);
        return ans;
    }

    rational rational::operator +(int n) {
        rational q2 = rational(n);
        return *this + q2;
    }

    rational rational::operator -(rational q2) {
        q2[0] *= -1;
        return *this + q2;
    }

    rational rational::operator -(int n) {
        rational q2 = rational(n);
        return *this - q2;
    }

    rational rational::operator *(rational q2) {
        int den = q[1] * q2[1];
        int num = q[0] * q2[0];
        rational ans(num, den);
        return ans;
    }

    rational rational::operator *(int n) {
        rational q2(n);
        return *this * q2;
    }

    rational rational::operator /(rational q2) {
        rational q2_inv(q2[1], q2[0]);
        return *this * q2_inv;
    }

    rational rational::operator /(int n) {
        rational q2(n);
        rational q2_inv(q[1], q[0]);
        return *this * q2_inv;
    }

    int rational::sign() {
        return sgn(q[0]);
    }

    bool rational::operator >(rational q2) {
        return !(*this <= q2);
    }

    bool rational::operator <(rational q2) {
        return !(*this >= q2);
    }

    bool rational::operator >=(rational q2) {
        return (*this - q2).sign() >= 0 ? true : false;
    }

    bool rational::operator <= (rational q2) {
        return (*this - q2).sign() <= 0 ? true : false;
    }


    double rational::dec() {
        return 1.0 * q[0] / q[1];
    }

    int rational::floor() {
        
        return  (q[0] > 0) ? (q[0] / q[1]): (q[0] - 1) / q[1] - 1;
    }

    rational rational::frac() {
        return *this - this->floor();
    }

    int rational::ceil() {
        return  (q[0] > 0) ? 1 + (q[0] - 1) / q[1] : (q[0] /q[1]);
    }

    int& rational::num() {
        return q[0];
    }

    int& rational::den() {
        return q[1];
    }