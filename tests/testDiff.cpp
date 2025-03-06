#include <cassert>
#include <complex>
#include <iostream>
#include <ostream>

#include "../inc/Expression.h"
#include "../inc/Parser.h"

void TestDiffDouble(std::string input, std::string expected, std::string param) {
    auto tmp = tokenize(input);
    auto value = parseExpression<double>(tmp.get());
    auto result = value->diff(param);
    std::cout << input << std::endl;
    std::cout << result->toString() << std::endl;
    std::cout << expected << std::endl;
    std::cout << std::endl;
    assert(result->toString() == expected);
}

void TestDiffComplex(std::string input, std::string expected, std::string param) {
    auto tmp = tokenize(input);
    auto value = parseExpression<std::complex<double>>(tmp.get());
    auto result = value->diff(param);
    std::cout << input << std::endl;
    std::cout << result->toString() << std::endl;
    std::cout << expected << std::endl;
    std::cout << std::endl;
    assert(result->toString() == expected);
}

int main() {
    TestDiffDouble("x", "1", "x");
    TestDiffDouble("x + y", "1", "x");
    TestDiffDouble("2", "0", "x");
    TestDiffDouble("2 * x", "(2 * 1)", "x");
    TestDiffDouble("x ^ 2", "((x ^ 2) * ((0 * ln(2)) + (2 * (1 / x))))", "x");
    TestDiffDouble("3 * x + 5", "(3 * 1)", "x");
    TestDiffDouble("5 - x * 3", "(-1 * (1 * 3))", "x");
    TestDiffDouble("3 * x * 5", "((3 * 1) * 5)", "x");
    TestDiffDouble("5 / x", "((5 * 1) * (x ^ 2))", "x");
    TestDiffDouble("5 / (x - 2)", "((5 * 1) * ((x - 2) ^ 2))", "x");
    TestDiffDouble("sin(x)", "(cos(x) * 1)", "x");
    TestDiffDouble("ln(x)", "(1 / x)", "x");
    TestDiffDouble("x * sin(x)", "((1 * sin(x)) + (x * (cos(x) * 1)))", "x");
    TestDiffDouble("x * sin(cos(x))", "((1 * sin(cos(x))) + (x * (cos(cos(x)) * ((-1 * sin(x)) * 1))))", "x");
    TestDiffDouble("exp(sin(x)", "((cos(x) * 1) * exp(sin(x)))", "x");

    TestDiffComplex("x", "(1,0)", "x");
    TestDiffComplex("x + y", "(1,0)", "x");
    TestDiffComplex("2", "(0,0)", "x");
    TestDiffComplex("2 * x", "((2,0) * (1,0))", "x");
    TestDiffComplex("x ^ 2", "((x ^ (2,0)) * (((0,0) * ln((2,0))) + ((2,0) * ((1,0) / x))))", "x");
    TestDiffComplex("3 * x + 5", "((3,0) * (1,0))", "x");
    TestDiffComplex("5 - x * 3", "((-1,0) * ((1,0) * (3,0)))", "x");
    TestDiffComplex("3 * x * 5", "(((3,0) * (1,0)) * (5,0))", "x");
    TestDiffComplex("5 / x", "(((5,0) * (1,0)) * (x ^ (2,0)))", "x");
    TestDiffComplex("5 / (x - 2)", "(((5,0) * (1,0)) * ((x - (2,0)) ^ (2,0)))", "x");
    TestDiffComplex("sin(x)", "(cos(x) * (1,0))", "x");
    TestDiffComplex("ln(x)", "((1,0) / x)", "x");
    TestDiffComplex("x * sin(x)", "(((1,0) * sin(x)) + (x * (cos(x) * (1,0))))", "x");
    TestDiffComplex("x * sin(cos(x))", "(((1,0) * sin(cos(x))) + (x * (cos(cos(x)) * (((-1,0) * sin(x)) * (1,0)))))", "x");
    TestDiffComplex("exp(sin(x)", "((cos(x) * (1,0)) * exp(sin(x)))", "x");
}