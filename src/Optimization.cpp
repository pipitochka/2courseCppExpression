#include "../inc/Expression.h"

std::shared_ptr<Expression> optimize(std::shared_ptr<Expression> expr){
    if (auto expr_binary = dynamic_cast<BinaryExpression*>(expr.get())) {
        expr_binary->lhs = optimize(expr_binary->lhs);
        expr_binary->rhs = optimize(expr_binary->rhs);
        auto lhs_const = dynamic_cast<ConstantExpression *>((expr_binary->lhs).get());
        auto rhs_const = dynamic_cast<ConstantExpression *>((expr_binary->rhs).get());
        switch (expr_binary->op.value) {
            case plus_op: {
                if (lhs_const && rhs_const) {
                    return std::make_shared<ConstantExpression>(lhs_const->getValue() + rhs_const->getValue());
                }
                if (lhs_const && lhs_const->getValue() == 0) {
                    return expr_binary->rhs;
                }
                if (rhs_const && rhs_const->getValue() == 0) {
                    return expr_binary->lhs;
                }
                break;
            }
            case minus_op: {
                if (lhs_const && rhs_const) {
                    return std::make_shared<ConstantExpression>(lhs_const->getValue() - rhs_const->getValue());
                }
                if (lhs_const && lhs_const->getValue() == 0) {
                    return expr_binary->rhs;
                }
                if (rhs_const && rhs_const->getValue() == 0) {
                    return std::make_shared<BinaryExpression>(
                        std::make_shared<ConstantExpression>(-1),
                        expr_binary->rhs,
                        mul_op
                    );
                }
                break;
            }
            case mul_op: {
                if (lhs_const && rhs_const) {
                    return std::make_shared<ConstantExpression>(lhs_const->getValue() * rhs_const->getValue());
                }
                if (lhs_const && lhs_const->getValue() == 0 || rhs_const && rhs_const->getValue() == 0) {
                    return std::make_shared<ConstantExpression>(0);
                }
                if (lhs_const && lhs_const->getValue() == 1) {
                    return expr_binary->rhs;
                }
                if (rhs_const && rhs_const->getValue() == 1) {
                    return expr_binary->lhs;
                }
                break;
            }
            case div_op: {
                if (rhs_const && rhs_const->getValue() == 0) {
                    throw ZeroDivExeption();
                }
                if (lhs_const && rhs_const) {
                    return std::make_shared<ConstantExpression>(lhs_const->getValue() / rhs_const->getValue());
                }
                if (lhs_const && lhs_const->getValue() == 0) {
                    return std::make_shared<ConstantExpression>(0);
                }
                break;
            }
            case exp_op: {
                if (lhs_const && lhs_const->getValue() == 0) {
                    throw ZeroExpExeption();
                }
                if (rhs_const && rhs_const->getValue() == 0) {
                    return std::make_shared<ConstantExpression>(1);
                }
                if (lhs_const && rhs_const) {
                    return std::make_shared<ConstantExpression>(std::pow(lhs_const->getValue(), rhs_const->getValue()));
                }
                break;
            }
        }
    }
    if (auto expr_mono = dynamic_cast<MonoExpression*>(expr.get())) {
        if (auto val = dynamic_cast<ConstantExpression *>(expr_mono->expr.get())) {
            expr_mono->expr = optimize(expr_mono->expr);
            switch (expr_mono->function) {
                case sin_func:
                    return std::make_shared<ConstantExpression>(std::sin(val->getValue()));
                case cos_func:
                    return std::make_shared<ConstantExpression>(std::cos(val->getValue()));
                case exp_func:
                    return std::make_shared<ConstantExpression>(std::exp(val->getValue()));
                case ln_func:
                    return std::make_shared<ConstantExpression>(std::log(val->getValue()));
            }
        }
    }
    return expr;
}