#include <cassert>
#include <iostream>
#include <ostream>

#include "../inc/Expression.h"
#include "../inc/Parser.h"

void TestInput(std::string input, std::string expected) {
    auto tmp = tokenize(input);
    auto var = parseExpression(tmp.get());
    auto result = var.first;
    std::cout << input << std::endl;
    std::cout << result->toString() << std::endl;
    std::cout << expected << std::endl;
    std::cout << std::endl;
    assert(result->toString() == expected);
}

int main() {
    TestInput("x", "x");
    TestInput("x + y", "(x + y)");
    TestInput("sin(cos(x + y))", "sin(cos((x + y)))");
    TestInput("2 + 3", "(2.000000 + 3.000000)");
    TestInput("2 + 3 * 5", "(2.000000 + (3.000000 * 5.000000))");
    TestInput("1 * 2 + 3 * 4", "((1.000000 * 2.000000) + (3.000000 * 4.000000))");
    TestInput("2 ^ 3 + 4", "((2.000000 ^ 3.000000) + 4.000000)");
    TestInput("2 ^ 3 * 5 + 4", "(((2.000000 ^ 3.000000) * 5.000000) + 4.000000)");
    TestInput("2 ^ (3 * 5) + 4", "((2.000000 ^ (3.000000 * 5.000000)) + 4.000000)");
    TestInput("x ^ y + t", "((x ^ y) + t)");
    TestInput("a * (b * c + d)", "(a * ((b * c) + d))");
    TestInput("z ^ (x ^ y + t)", "(z ^ ((x ^ y) + t))");
    TestInput("2 + 3 * 4", "(2.000000 + (3.000000 * 4.000000))");
    TestInput("2 * 3 + 4", "((2.000000 * 3.000000) + 4.000000)");
    TestInput("2 + 3 * sin(cos(x + y))", "(2.000000 + (3.000000 * sin(cos((x + y)))))");
    TestInput("2 * ln(x) + 3", "((2.000000 * ln(x)) + 3.000000)");
    TestInput("2 * (ln(x)) + 3 * sin(cos(x + y))", "((2.000000 * ln(x)) + (3.000000 * sin(cos((x + y)))))");
    TestInput("x ^ ln(y) + exp(z) ^ (cos(t) + sin(m))", "((x ^ ln(y)) + (exp(z) ^ (cos(t) + sin(m))))");
    TestInput("a * b / c", "(a * (b / c))");
}

