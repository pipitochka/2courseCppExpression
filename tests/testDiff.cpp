#include <cassert>
#include <iostream>
#include <ostream>

#include "../inc/Expression.h"
#include "../inc/Parser.h"

void TestDiff(std::string input, std::string expected, std::string param) {
    auto tmp = tokenize(input);
    auto value = parseExpression(tmp.get());
    auto result = value->diff(param);
    std::cout << input << std::endl;
    std::cout << result->toString() << std::endl;
    std::cout << expected << std::endl;
    std::cout << std::endl;
    assert(result->toString() == expected);
}

int main() {
    TestDiff("x", "1.000000", "x");
    TestDiff("x + y", "1.000000", "x");
    TestDiff("2", "0.000000", "x");
    TestDiff("2 * x", "(2.000000 * 1.000000)", "x");
    TestDiff("x ^ 2", "((x ^ 2.000000) * ((0.000000 * ln(2.000000)) + (2.000000 * (1.000000 / x))))", "x");
    //TestDiff(, "", "x");
    TestDiff("3 * x + 5", "(3.000000 * 1.000000)", "x");
    TestDiff("5 - x * 3", "(-1.000000 * (1.000000 * 3.000000))", "x");
    TestDiff("3 * x * 5", "((3.000000 * 1.000000) * 5.000000)", "x");
    TestDiff("5 / x", "((5.000000 * 1.000000) * (x ^ 2.000000))", "x");
    TestDiff("5 / (x - 2)", "((5.000000 * 1.000000) * ((x - 2.000000) ^ 2.000000))", "x");
    TestDiff("sin(x)", "(cos(x) * 1.000000)", "x");
    TestDiff("ln(x)", "(1.000000 / x)", "x");
    TestDiff("x * sin(x)", "((1.000000 * sin(x)) + (x * (cos(x) * 1.000000)))", "x");
    TestDiff("x * sin(cos(x))", "((1.000000 * sin(cos(x))) + (x * (cos(cos(x)) * ((-1.000000 * sin(x)) * 1.000000))))", "x");
    TestDiff("exp(sin(x)", "((cos(x) * 1.000000) * exp(sin(x)))", "x");
}