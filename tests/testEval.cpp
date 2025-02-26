#include <cassert>
#include <iostream>
#include <ostream>

#include "../inc/Expression.h"
#include "../inc/Parser.h"

void TestEval(std::string input, double expected, std::map<std::string, double> params) {
    auto tmp = tokenize(input);
    auto result = parseExpression(tmp.get());
    auto qq = result->eval(params);
    std::cout << input << std::endl;
    std::cout << qq << std::endl;
    std::cout << expected << std::endl;
    std::cout << std::endl;
    assert(qq == expected);
}

int main() {
    TestEval("5 + 7", 12, {});
    TestEval("2.5", 2.5, {});
    TestEval("x + 5", 7, {{"x", 2}});
    TestEval("x ^ 5", 32, {{"x", 2}});
    TestEval("x + y", 12, {{"x", 5}, {"y", 7}});
    TestEval("x ^ y", 125, {{"x", 5}, {"y", 3}});
    TestEval("x / y", 2, {{"x", 10}, {"y", 5}});
    TestEval("sin(x)", 0, {{"x", 0}, {"y", 5}});
    TestEval("cos(x)", 1, {{"x", 0}});  
    TestEval("exp(x)", exp(1), {{"x", 1}});
    TestEval("x * sin(x)", 3 * sin(3.0), {{"x", 3}});
    TestEval("x * y + sin(x)", 5 * 2 + sin(5), {{"x", 5}, {"y", 2}});
}

