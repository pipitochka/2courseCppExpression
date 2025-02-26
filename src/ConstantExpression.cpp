#include "../inc/Expression.h"

std::shared_ptr<Expression> ConstantExpression::diff(std::string& s) {
    return std::make_shared<ConstantExpression>(ConstantExpression(0));
}

double ConstantExpression::eval(std::map<std::string, int> &params) {
    return value;
}

std::string ConstantExpression::toString() {
    return std::to_string(value);
}