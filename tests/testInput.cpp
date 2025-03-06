#include <cassert>
#include <complex>
#include <iostream>
#include <ostream>
#include <string>

#include "../inc/Expression.h"
#include "../inc/Parser.h"

void TestInputDouble(std::string input, std::string expected) {
    auto tmp = tokenize(input);
    auto result = parseExpression<double>(tmp.get());
    std::cout << input << std::endl;
    std::cout << result->toString() << std::endl;
    std::cout << expected << std::endl;
    std::cout << std::endl;
    assert(result->toString() == expected);
}

void TestInputComplex(std::string input, std::string expected) {
    auto tmp = tokenize(input);
    auto result = parseExpression<std::complex<double>>(tmp.get());
    std::cout << input << std::endl;
    std::cout << result->toString() << std::endl;
    std::cout << expected << std::endl;
    std::cout << std::endl;
    assert(result->toString() == expected);
}

int main() {
    TestInputDouble("x", "x");
    TestInputDouble("x + y", "(x + y)");
    TestInputDouble("sin(cos(x + y))", "sin(cos((x + y)))");
    TestInputDouble("2 + 3", "(2 + 3)");
    TestInputDouble("2 + 3 * 5", "(2 + (3 * 5))");
    TestInputDouble("1 * 2 + 3 * 4", "((1 * 2) + (3 * 4))");
    TestInputDouble("2 ^ 3 + 4", "((2 ^ 3) + 4)");
    TestInputDouble("2 ^ 3 * 5 + 4", "(((2 ^ 3) * 5) + 4)");
    TestInputDouble("2 ^ (3 * 5) + 4", "((2 ^ (3 * 5)) + 4)");
    TestInputDouble("x ^ y + t", "((x ^ y) + t)");
    TestInputDouble("a * (b * c + d)", "(a * ((b * c) + d))");
    TestInputDouble("z ^ (x ^ y + t)", "(z ^ ((x ^ y) + t))");
    TestInputDouble("2 + 3 * 4", "(2 + (3 * 4))");
    TestInputDouble("2 * 3 + 4", "((2 * 3) + 4)");
    TestInputDouble("2 + 3 * sin(cos(x + y))", "(2 + (3 * sin(cos((x + y)))))");
    TestInputDouble("2 * ln(x) + 3", "((2 * ln(x)) + 3)");
    TestInputDouble("2 * (ln(x)) + 3 * sin(cos(x + y))", "((2 * ln(x)) + (3 * sin(cos((x + y)))))");
    TestInputDouble("x ^ ln(y) + exp(z) ^ (cos(t) + sin(m))", "((x ^ ln(y)) + (exp(z) ^ (cos(t) + sin(m))))");
    TestInputDouble("a / b * c", "((a / b) * c)");
    TestInputDouble("a * b / c", "((a * b) / c)");


    TestInputComplex("x", "x");
    TestInputComplex("x + y", "(x + y)");
    TestInputComplex("sin(cos(x + y))", "sin(cos((x + y)))");
    TestInputComplex("2 + 3", "((2,0) + (3,0))");
    TestInputComplex("2 + 3 * 5", "((2,0) + ((3,0) * (5,0)))");
    TestInputComplex("1 * 2 + 3 * 4", "(((1,0) * (2,0)) + ((3,0) * (4,0)))");
    TestInputComplex("2 ^ 3 + 4", "(((2,0) ^ (3,0)) + (4,0))");
    TestInputComplex("2 ^ 3 * 5 + 4", "((((2,0) ^ (3,0)) * (5,0)) + (4,0))");
    TestInputComplex("2 ^ (3 * 5) + 4", "(((2,0) ^ ((3,0) * (5,0))) + (4,0))");
    TestInputComplex("x ^ y + t", "((x ^ y) + t)");
    TestInputComplex("a * (b * c + d)", "(a * ((b * c) + d))");
    TestInputComplex("z ^ (x ^ y + t)", "(z ^ ((x ^ y) + t))");
    TestInputComplex("2 + 3 * 4", "((2,0) + ((3,0) * (4,0)))");
    TestInputComplex("2 * 3 + 4", "(((2,0) * (3,0)) + (4,0))");
    TestInputComplex("2 + 3 * sin(cos(x + y))", "((2,0) + ((3,0) * sin(cos((x + y)))))");
    TestInputComplex("2 * ln(x) + 3", "(((2,0) * ln(x)) + (3,0))");
    TestInputComplex("2 * (ln(x)) + 3 * sin(cos(x + y))", "(((2,0) * ln(x)) + ((3,0) * sin(cos((x + y)))))");
    TestInputComplex("x ^ ln(y) + exp(z) ^ (cos(t) + sin(m))", "((x ^ ln(y)) + (exp(z) ^ (cos(t) + sin(m))))");
    TestInputComplex("a / b * c", "((a / b) * c)");
    TestInputComplex("a * b / c", "((a * b) / c)");
}

