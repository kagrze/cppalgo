#include <iostream>
#include <cassert>
#include <cmath>

/*
 * Recursive power 
 */
float power(float base, unsigned int exponent) {
	if (exponent == 0)
		return 1;
	else
		return power(base, exponent -1) * base;
}

void test_power() {
	assert(power(2, 0) == 1);
	assert(power(2, 1) == 2);
	assert(power(2, 2) == 4);
	assert(power(2, 3) == 8);

	assert(power(0.5, 0) == 1);
	assert(power(0.5, 1) == 0.5);
	assert(power(0.5, 2) == 0.25);
	assert(power(0.5, 3) == 0.125);
}

auto margin = 0.001;

/*
 * The Babylonian method for square root. a.k.a. the Heron's method
 */
float sqrtRecursive(float input, float guess) {
	if (std::abs(guess * guess - input) / input < margin)
		return guess;
	else
		return sqrtRecursive(input, (guess + input / guess) / 2);
}

float squareRoot(float input) {
	return sqrtRecursive(input, 1);
}

void test_squareRoot() {
	auto root = squareRoot(9);
	auto expected = 3.0;
	assert(root > expected - margin && root < expected + margin);

	root = squareRoot(4);
	expected = 2.0;
	assert(root > expected - margin && root < expected + margin);

	root = squareRoot(2);
	expected = 1.414;
	assert(root > expected - margin && root < expected + margin);
}

int main() {
	test_power();
	test_squareRoot();
	std::cout << "All tests have passed" << std::endl;
}