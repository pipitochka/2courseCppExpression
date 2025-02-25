#include <iostream>
#include "inc/Expression.h"
#include "inc/Token.h"
#include "inc/Parser.h"

int main() {
    // std::string input1 = "x * sin(x)";
    // auto token1 = tokenize(input1);
    // printToken(token1.get());
    // std::cout << std::endl;
    //
    // auto q = parseExpression(token1.get());
    // std::cout << q.first->toString() << std::endl;
    //
    // std::string input2 = "x * y";
    // auto token2 = tokenize(input2);
    // printToken(token2.get());
    // std::cout << std::endl;
    //
    // auto q2 = parseExpression(token2.get());
    // std::cout << q2.first->toString() << std::endl;
    
    std::string input3 = "sin(cos(x)) + exp(ln(y)) ^ n";
    auto token3 = tokenize(input3);
    printToken(token3.get());
    std::cout << std::endl;

    auto q3 = parseExpression(token3.get());
    std::cout << q3.first->toString() << std::endl;
    std::cout << q3.first->diff()->toString() << std::endl;
    
    // BinaryExpression expression(std::make_shared<ConstantExpression>(ConstantExpression(1)),
    //     std::make_shared<VarExpression>(VarExpression("x")),
    //     plus_op);
    //
    // std::map<std::string, int> map = {{"x", 3}}; 
    // std::shared_ptr<Expression> derivative = expression.diff();
    // double derivative2 = expression.eval(map);
    // auto binaryDerivative = std::dynamic_pointer_cast<BinaryExpression>(derivative);
    //
    // std::cout << binaryDerivative->toString() << std::endl;
    //
    // std::cout << derivative2 << std::endl;
}
