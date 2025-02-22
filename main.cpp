#include <iostream>
#include "inc/Expression.h"

int main() {
    BinaryExpression expression(std::make_shared<ConstantExpression>(ConstantExpression(1)),
        std::make_shared<VarExpression>(VarExpression("x")),
        plus_op);

    std::shared_ptr<Expression> derivative = expression.diff();

    auto binaryDerivative = std::dynamic_pointer_cast<BinaryExpression>(derivative);

    std::cout << binaryDerivative << std::endl;
}
