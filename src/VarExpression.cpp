#include "../inc/Expression.h"

std::shared_ptr<Expression> VarExpression::diff() {
    return std::make_unique<ConstantExpression>(ConstantExpression(1));
}

double VarExpression::eval(std::map<std::string, int> &map) {
    return map[value];
}