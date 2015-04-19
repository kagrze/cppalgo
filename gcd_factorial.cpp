#include <iostream>
#include <cassert>
#include <cmath>

/*
 * Computing greatest common divisor (GCD) with the Euclidean algorithm
 */
int gcd(int a, int b) {
    if(a == 0) return b;
    if(b == 0) return a;
    return gcd(std::abs(a - b), a < b ? a : b);
}

void testGcd() {
    assert(gcd(0, 1) == 1);
    assert(gcd(1, 0) == 1);
    assert(gcd(1, 1) == 1);
    assert(gcd(0, 0) == 0);
    assert(gcd(4, 6) == 2);
    assert(gcd(252, 105) == 21);
    assert(gcd(105, 252) == 21);
    assert(gcd(270, 192) == 6);
    assert(gcd(192, 270) == 6);
}

/*
 * Computing factorial for a given input number
 */
unsigned long factorial(unsigned int input) {
    return (input == 0) ? 1 : factorial(input - 1) * input;
}

void testFactorial() {
    assert(factorial(0) == 1);
    assert(factorial(1) == 1);
    assert(factorial(2) == 2);
    assert(factorial(3) == 6);
    assert(factorial(4) == 24);
    assert(factorial(5) == 120);
}

int main() {
    testGcd();
    testFactorial();
    std::cout << "All tests have passed" << std::endl;
}