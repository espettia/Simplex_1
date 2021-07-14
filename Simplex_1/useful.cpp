#include <iostream>
#include "useful.h"

int sgn(int a) {
    return (0 < a) - (a < 0);
}

int gcd(int a, int b)
{
    a = abs(a);
    b = abs(b);
    int tmp;
    while (b != 0) {
        tmp = a;
        a = b;
        b = tmp % b;
    }
    return a;     
}

int lcm(int a, int b) {
    return a * b / gcd(a, b);
}