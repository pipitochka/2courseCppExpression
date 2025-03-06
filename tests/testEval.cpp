#include <cassert>
#include <complex>
#include <iostream>
#include <ostream>

#include "../inc/Expression.h"
#include "../inc/Parser.h"

void TestEvalDouble(std::string input, double expected, std::map<std::string, double> params) {
    auto tmp = tokenize(input);
    auto result = parseExpression<double>(tmp.get());
    auto qq = result->eval(params);
    std::cout << input << std::endl;
    std::cout << qq << std::endl;
    std::cout << expected << std::endl;
    std::cout << std::endl;
    assert(qq == expected);
}

void TestEvalComplex(std::string input, std::complex<double> expected, std::map<std::string, double> params) {
    auto tmp = tokenize(input);
    auto result = parseExpression<std::complex<double>>(tmp.get());
    auto qq = result->eval(params);
    std::cout << input << std::endl;
    std::cout << qq << std::endl;
    std::cout << expected << std::endl;
    std::cout << std::endl;
    assert(qq == expected);
}

int main() {
    TestEvalDouble("5 + 7", 12, {});
    TestEvalDouble("2.5", 2.5, {});
    TestEvalDouble("x + 5", 7, {{"x", 2}});
    TestEvalDouble("x ^ 5", 32, {{"x", 2}});
    TestEvalDouble("x + y", 12, {{"x", 5}, {"y", 7}});
    TestEvalDouble("x ^ y", 125, {{"x", 5}, {"y", 3}});
    TestEvalDouble("x / y", 2, {{"x", 10}, {"y", 5}});
    TestEvalDouble("sin(x)", 0, {{"x", 0}, {"y", 5}});
    TestEvalDouble("cos(x)", 1, {{"x", 0}});  
    TestEvalDouble("exp(x)", exp(1), {{"x", 1}});
    TestEvalDouble("x * sin(x)", 3 * sin(3.0), {{"x", 3}});
    TestEvalDouble("x * y + sin(x)", 5 * 2 + sin(5), {{"x", 5}, {"y", 2}});

    TestEvalComplex("5 + 7", std::complex<double>(12,0), {});
    TestEvalComplex("2.5", std::complex<double>(2.5,0), {});
    TestEvalComplex("x + 5", std::complex<double>(7,0), {{"x", 2}});
    TestEvalComplex("x ^ 5", std::complex<double>(32,0), {{"x", 2}});
    TestEvalComplex("x + y", std::complex<double>(12,0), {{"x", 5}, {"y", 7}});
    //TestEvalComplex("x ^ y", std::complex<double>(125,0), {{"x", 5}, {"y", 3}});
    TestEvalComplex("x / y", 2, {{"x", 10}, {"y", 5}});
    TestEvalComplex("sin(x)", 0, {{"x", 0}, {"y", 5}});
    TestEvalComplex("cos(x)", 1, {{"x", 0}});  
    TestEvalComplex("exp(x)", exp(1), {{"x", 1}});
    TestEvalComplex("x * sin(x)", 3 * sin(3.0), {{"x", 3}});
    TestEvalComplex("x * y + sin(x)", 5 * 2 + sin(5), {{"x", 5}, {"y", 2}});
}

