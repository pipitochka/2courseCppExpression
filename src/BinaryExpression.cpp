#include "../inc/Expression.h"

std::shared_ptr<Expression> BinaryExpression::diff(std::string &s) {
    auto lhs_diff = lhs->diff(s);
    auto rhs_diff = rhs->diff(s);
    auto lhs_const = dynamic_cast<ConstantExpression*>(lhs.get());
    auto rhs_const = dynamic_cast<ConstantExpression*>(rhs.get());
    auto lhs_diff_const = dynamic_cast<ConstantExpression*>(lhs_diff.get());
    auto rhs_diff_const = dynamic_cast<ConstantExpression*>(rhs_diff.get());
    switch (op.value) {
        case plus_op: {
            if (lhs_diff_const && rhs_diff_const && lhs_diff_const->getValue() == 0 && rhs_diff_const->getValue() == 0) {
                return std::make_shared<ConstantExpression>(0);
            }
            if (lhs_diff_const && lhs_diff_const->getValue() == 0) {
                return rhs_diff;
            }
            if (rhs_diff_const && rhs_diff_const->getValue() == 0) {
                return lhs_diff;
            }
            return std::make_shared<BinaryExpression>(lhs_diff, rhs_diff, plus_op);
        }
        case minus_op: {
            if (lhs_diff_const && rhs_diff_const && lhs_diff_const->getValue() == 0 && rhs_diff_const->getValue() == 0) {
                return std::make_shared<ConstantExpression>(0);
            }
            if (lhs_diff_const && lhs_diff_const->getValue() == 0) {
                return std::make_shared<BinaryExpression>(std::make_shared<ConstantExpression>(-1), rhs_diff, mul_op);
            }
            if (rhs_diff_const && rhs_diff_const->getValue() == 0) {
                return lhs_diff;
            }
            return std::make_shared<BinaryExpression>(lhs_diff, rhs_diff, minus_op);
        }
        case mul_op: {
            if (lhs_const && rhs_const && lhs_const->getValue() == 0 && rhs_const->getValue() == 0) {
                return std::make_shared<ConstantExpression>(0);
            }
            if (lhs_const && lhs_const->getValue() == 0) {
                return std::make_shared<ConstantExpression>(0);
            }
            if (rhs_const && rhs_const->getValue() == 0) {
                return std::make_shared<ConstantExpression>(0);
            }
            if (lhs_diff_const && rhs_diff_const && lhs_diff_const->getValue() == 0
                && rhs_diff_const->getValue() == 0) {
                return std::make_shared<ConstantExpression>(0);
            }
            if (lhs_diff_const && lhs_diff_const->getValue() == 0) {
                return std::make_shared<BinaryExpression>(lhs, rhs_diff, mul_op);
            }
            if (rhs_diff_const && rhs_diff_const->getValue() == 0) {
                return std::make_shared<BinaryExpression>(lhs_diff, rhs, mul_op);
            }
            return std::make_shared<BinaryExpression>(
                std::make_shared<BinaryExpression>(lhs_diff, rhs, mul_op),
                std::make_shared<BinaryExpression>(lhs, rhs_diff, mul_op),
                plus_op);
        }
        case div_op: {
            if (rhs_const && rhs_const->getValue() == 0) {
                throw ZeroDivExeption();
            }
            auto t = std::make_shared<BinaryExpression>(lhs, rhs, mul_op);
            auto t_diff = t->diff(s);
            return std::make_shared<BinaryExpression>(t_diff,
                std::make_shared<BinaryExpression>(rhs, std::make_shared<ConstantExpression>(2), exp_op),
                mul_op);
        }
        case exp_op: {
            if (lhs_const && lhs_const->getValue() == 0) {
                throw ZeroExpExeption();
            }
            return std::make_shared<BinaryExpression>(
                std::make_shared<BinaryExpression>(
                    lhs,
                    rhs,
                    exp_op
                ),
                std::make_shared<BinaryExpression>(
                    std::make_shared<BinaryExpression>(
                        rhs_diff,
                        std::make_shared<MonoExpression>(
                            rhs,
                            ln_func
                        ),
                        mul_op),
                    std::make_shared<BinaryExpression>(
                        rhs,
                        std::make_shared<BinaryExpression>(
                            lhs_diff,
                            lhs,
                            div_op),
                        mul_op),
                    plus_op),
                mul_op);
        }
    }
    return nullptr;
}

double BinaryExpression::eval(std::map<std::string, int> &params) {
    switch (op.value) {
        case plus_op:
            if (lhs->eval(params) == 0 && rhs->eval(params) == 0) {
                return 0;
            }
            if (lhs->eval(params) == 0) {
                return rhs->eval(params);
            }
            if (rhs->eval(params) == 0) {
                return lhs->eval(params);
            }
            return lhs->eval(params) + rhs->eval(params);
        case minus_op:
            if (lhs->eval(params) == 0 && rhs->eval(params) == 0) {
                return 0;
            }
            if (lhs->eval(params) == 0) {
                return rhs->eval(params) * -1;
            }
            if (rhs->eval(params) == 0) {
                return lhs->eval(params);
            }
            return lhs->eval(params) - rhs->eval(params);
        case mul_op:
            if (lhs->eval(params) == 0 || rhs->eval(params) == 0) {
                return 0;
            }
            return lhs->eval(params) * rhs->eval(params);
        case div_op:
            if (rhs->eval(params) == 0.0) {
                throw ZeroDivExeption();
            }
            if (lhs->eval(params) == 0) {
                return 0;
            }
            return lhs->eval(params) / rhs->eval(params);
        case exp_op:
            if (lhs->eval(params) == 0 && rhs->eval(params) == 0) {
                throw ZeroDivExeption();
            }
            if (rhs->eval(params) == 0) {
                return 1;
            }
            if (rhs->eval(params) == 1) {
                return lhs->eval(params);
            }
            return std::pow(lhs->eval(params), rhs->eval(params));
    }
}

std::string BinaryExpression::toString() {
    std::string tmp;
    switch (op.value) {
        case plus_op:
            tmp = "+";
            break;
        case minus_op:
            tmp = "-";
            break;
        case mul_op:
            tmp = "*";
            break;
        case div_op:
            tmp = "/";
            break;
        case exp_op:
            tmp = "^";
            break;
    }
    return "(" + lhs->toString() + " " + tmp + " " + rhs->toString() + ")";
}