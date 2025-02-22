#include "../inc/Expression.h"

std::shared_ptr<Expression> MonoExpression::diff() {
    switch (function) {
        case cos_func:
            return std::make_shared<BinaryExpression>(BinaryExpression(
                std::make_shared<ConstantExpression>(ConstantExpression(-1.0)),
                std::make_shared<MonoExpression>(MonoExpression(expr, sin_func)),
                mul_op
            ));
        case sin_func:
            return std::make_shared<MonoExpression>(MonoExpression(expr, cos_func));
        case ln_func:
            return std::make_shared<BinaryExpression>(BinaryExpression(
                expr->diff(),
                expr,
                div_op));
        case exp_func:
            return std::make_shared<BinaryExpression>(BinaryExpression(
                expr->diff(),
                std::make_shared<MonoExpression>(MonoExpression(expr, function)),
                mul_op));
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