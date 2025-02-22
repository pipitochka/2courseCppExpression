#include "../inc/Expression.h"

std::shared_ptr<Expression> BinaryExpression::diff() {
    switch (op) {
        case plus_op:
        case minus_op:
            return std::make_shared<BinaryExpression>(BinaryExpression(lhs->diff(), rhs->diff(), op));
        case mul_op:
            return std::make_shared<BinaryExpression>(
                BinaryExpression(
                    std::make_shared<BinaryExpression>(BinaryExpression(lhs, rhs->diff(), mul_op)),
                    std::make_shared<BinaryExpression>(BinaryExpression(lhs->diff(), rhs, mul_op)),
                    plus_op));
        case div_op:
            return std::make_shared<BinaryExpression>(
                BinaryExpression(
                    std::make_shared<BinaryExpression>(BinaryExpression(
                        std::make_shared<BinaryExpression>(BinaryExpression(lhs, rhs->diff(), mul_op)),
                        std::make_shared<BinaryExpression>(BinaryExpression(lhs->diff(), rhs, mul_op)),
                        mul_op)),
                    std::make_shared<BinaryExpression>(BinaryExpression(
                        rhs,
                        rhs,
                        mul_op)),
                    div_op));
        case exp_op:
            return std::make_shared<BinaryExpression>(BinaryExpression(
                std::make_shared<BinaryExpression>(BinaryExpression(
                    lhs,
                    rhs,
                    exp_op)),
                std::make_shared<BinaryExpression>(BinaryExpression(
                    std::make_shared<BinaryExpression>(BinaryExpression(
                        rhs->diff(),
                        std::make_shared<MonoExpression>(MonoExpression(
                            lhs, ln_func)),
                        mul_op)),
                    std::make_shared<BinaryExpression>(BinaryExpression(
                        rhs,
                        std::make_shared<BinaryExpression>(BinaryExpression(
                            lhs->diff(),
                            lhs,
                            div_op)),
                        mul_op)),
                    plus_op)),
                mul_op));
    }
    return nullptr;
}

double BinaryExpression::eval(std::map<std::string, int> &params) {
    switch (op) {
        case plus_op:
            return lhs->eval(params) + rhs->eval(params);
        case minus_op:
            return lhs->eval(params) - rhs->eval(params);
        case mul_op:
            return lhs->eval(params) * rhs->eval(params);
        case div_op:
            if (rhs->eval(params) == 0.0) {
                throw ZeroDivExeption();
            }
            return lhs->eval(params) / rhs->eval(params);
        case exp_op:
            return std::pow(lhs->eval(params), rhs->eval(params));
    }
}
