#include "../inc/Expression.h"

std::shared_ptr<Expression> MonoExpression::diff(std::string& s) {
    auto exp_diff = expr->diff(s);
    switch (function) {
        case cos_func: {
            return std::make_shared<BinaryExpression>(
                std::make_shared<BinaryExpression>(BinaryExpression(
                    std::make_shared<ConstantExpression>(ConstantExpression(-1.0)),
                    std::make_shared<MonoExpression>(MonoExpression(expr, sin_func)),
                mul_op)),
                exp_diff,
                mul_op);
        }
        case sin_func: {
            return std::make_shared<BinaryExpression>(
                std::make_shared<MonoExpression>(MonoExpression(expr, cos_func)),
                exp_diff,
                mul_op);
        }
        case ln_func: {
            return std::make_shared<BinaryExpression>(BinaryExpression(
                exp_diff,
                expr,
                div_op));
        }
        case exp_func: {
            return std::make_shared<BinaryExpression>(BinaryExpression(
                exp_diff,
                std::make_shared<MonoExpression>(MonoExpression(expr, function)),
                mul_op));
        }
    }
    return nullptr;
}

double MonoExpression::eval(std::map<std::string, int> &params) {
    switch (function) {
        case cos_func:
            return std::cos(expr->eval(params));
        case sin_func:
            return std::sin(expr->eval(params));
        case ln_func:
            return std::log(expr->eval(params));
        case exp_func:
            return std::exp(expr->eval(params));
    }
}

std::string MonoExpression::toString() {
    std::string tmp;
    switch (function) {
        case cos_func:
            tmp = "cos";
            break;
        case sin_func:
            tmp = "sin";
            break;
        case ln_func:
            tmp = "ln";
            break;
        case exp_func:
            tmp = "exp";
            break;
    }
    return tmp + "(" + expr->toString() + ")";
}