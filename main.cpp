#include <iostream>
#include "inc/Expression.h"

int main() {
    BinaryExpression expression(std::make_shared<ConstantExpression>(ConstantExpression(1)),
        std::make_shared<VarExpression>(VarExpression("x")),
        plus_op);

    std::map<std::string, int> map = {{"x", 3}}; 
    std::shared_ptr<Expression> derivative = expression.diff();
    double derivative2 = expression.eval(map);
    auto binaryDerivative = std::dynamic_pointer_cast<BinaryExpression>(derivative);

    std::cout << binaryDerivative->toString() << std::endl;

    std::cout << derivative2 << std::endl;
}
