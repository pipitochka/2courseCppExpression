#include "../inc/Expression.h"

std::shared_ptr<Expression> VarExpression::diff(std::string &s) {
    if (s == value) {
        return std::make_shared<ConstantExpression>(ConstantExpression(1));
    }
    else {
        return std::make_shared<ConstantExpression>(0);
    }
}

double VarExpression::eval(std::map<std::string, double> &map) {
    return map[value]; 
}

std::string VarExpression::toString() {
    return value;
}