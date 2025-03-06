#pragma once
#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <map>
#include <string>
#include <set>
#include <memory>
#include <cmath>
#include <functional>
#include <functional>
#include <functional>
#include <sstream>


enum oper {plus_op, minus_op, div_op, mul_op, exp_op};
enum functions {sin_func, cos_func, ln_func, exp_func};

class operators {
public:
    oper value;
    int rank;
    operators(oper op) : value(op) {
        if (value == plus_op || value == minus_op) {
            rank = 1;
        }
        if (value == div_op || value == mul_op) {
            rank = 2;
        }
        if (value == exp_op) {
            rank = 3;
        }
    }
};

class ZeroDivExeption : public std::exception {
    public:
    const char* what() const noexcept override {
        return "Division by zero!";
    }
};

class ZeroExpExeption : public std::exception {
public:
    const char* what() const noexcept override {
        return "Zero exp zero!";
    }
};

template <typename T>
class Expression {
public:
    virtual std::shared_ptr<Expression> diff(std::string &s) = 0;
    virtual T eval(std::map<std::string, double> &params) = 0;
    virtual std::string toString() = 0;
};

template <typename T>
class ConstantExpression : public Expression<T> {
private:
    T value;
    
public:
    ConstantExpression(T value) : value(value) {};
    ~ConstantExpression() = default;
    ConstantExpression(const ConstantExpression&) = default;
    ConstantExpression& operator=(const ConstantExpression&) = default;
    ConstantExpression(ConstantExpression&&) = default;
    ConstantExpression& operator=(ConstantExpression&&) = default;

    T getValue() {
        return value;
    }
    T eval(std::map<std::string, double> &params) override;
    
    std::shared_ptr<Expression<T>> diff(std::string &s) override;
    
    std::string toString() override;
};

template <typename T>
class VarExpression : public Expression<T> {
public:
    std::string value;

public:
    VarExpression(const std::string value) : value(value) {};
    ~VarExpression() = default;
    VarExpression(const VarExpression&) = default;
    VarExpression& operator=(const VarExpression&) = default;
    VarExpression(VarExpression&&) = default;
    VarExpression& operator=(VarExpression&&) = default;

    T eval(std::map<std::string, double> &params) override;
    std::shared_ptr<Expression<T>> diff(std::string &s) override;
    std::string toString() override;
};

template <typename T>
class MonoExpression : public Expression<T> {
public:
    std::shared_ptr<Expression<T>> expr;
    functions function;
    
public:
    MonoExpression(const std::shared_ptr<Expression<T>> &expr, functions function) {
        this->expr = expr;
        this->function = function;
    }
    ~MonoExpression() = default;
    MonoExpression(const MonoExpression&) = default;
    MonoExpression& operator=(const MonoExpression&) = default;
    MonoExpression(MonoExpression&&) = default;
    MonoExpression& operator=(MonoExpression&&) = default;

    T eval(std::map<std::string, double> &params) override;
    std::shared_ptr<Expression<T>> diff(std::string &s) override;
    std::string toString() override;
    friend std::shared_ptr<Expression<T>> optimize(std::shared_ptr<Expression<T>> expr);
};

template <typename T>
class BinaryExpression : public Expression<T> {
public:
    std::shared_ptr<Expression<T>> lhs;
    std::shared_ptr<Expression<T>> rhs;
    operators op;
    
public:
    BinaryExpression(const std::shared_ptr<Expression<T>> &lhs, const std::shared_ptr<Expression<T>> &rhs, oper op): op(op) {
        this->lhs = lhs;
        this->rhs = rhs;
    }
    ~BinaryExpression() = default;
    BinaryExpression(const BinaryExpression&) = default;
    BinaryExpression& operator=(const BinaryExpression&) = default;
    BinaryExpression(BinaryExpression&&) = default;
    BinaryExpression& operator=(BinaryExpression&&) = default;

    std::shared_ptr<Expression<T>> getLhs() { return lhs; }
    void setLhs(std::shared_ptr<Expression<T>> lhs) { this->lhs = lhs; }
    operators getOp() { return op; }
    T eval(std::map<std::string, double> &params) override;
    std::shared_ptr<Expression<T>> diff(std::string &s) override;
    std::string toString() override;
    friend std::shared_ptr<Expression<T>> optimize(std::shared_ptr<Expression<T>> expr);
};

template <typename T>
std::shared_ptr<Expression<T>> optimize(std::shared_ptr<Expression<T>> expr) {
    if (auto expr_binary = dynamic_cast<BinaryExpression<T>*>(expr.get())) {
        expr_binary->lhs = optimize(expr_binary->lhs);
        expr_binary->rhs = optimize(expr_binary->rhs);
        auto lhs_const = dynamic_cast<ConstantExpression<T>*>((expr_binary->lhs).get());
        auto rhs_const = dynamic_cast<ConstantExpression<T>*>((expr_binary->rhs).get());
        switch (expr_binary->op.value) {
            case plus_op: {
                if (lhs_const && rhs_const) {
                    return std::make_shared<ConstantExpression<T>>(lhs_const->getValue() + rhs_const->getValue());
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
                    return std::make_shared<ConstantExpression<T>>(lhs_const->getValue() - rhs_const->getValue());
                }
                if (lhs_const && lhs_const->getValue() == 0) {
                    return expr_binary->rhs;
                }
                if (rhs_const && rhs_const->getValue() == 0) {
                    return std::make_shared<BinaryExpression<T>>(
                        std::make_shared<ConstantExpression<T>>(-1),
                        expr_binary->rhs,
                        mul_op
                    );
                }
                break;
            }
            case mul_op: {
                if (lhs_const && rhs_const) {
                    return std::make_shared<ConstantExpression<T>>(lhs_const->getValue() * rhs_const->getValue());
                }
                if (lhs_const && lhs_const->getValue() == 0 || rhs_const && rhs_const->getValue() == 0) {
                    return std::make_shared<ConstantExpression<T>>(0);
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
                    return std::make_shared<ConstantExpression<T>>(lhs_const->getValue() / rhs_const->getValue());
                }
                if (lhs_const && lhs_const->getValue() == 0) {
                    return std::make_shared<ConstantExpression<T>>(0);
                }
                break;
            }
            case exp_op: {
                if (lhs_const && lhs_const->getValue() == 0) {
                    throw ZeroExpExeption();
                }
                if (rhs_const && rhs_const->getValue() == 0) {
                    return std::make_shared<ConstantExpression<T>>(1);
                }
                if (lhs_const && rhs_const) {
                    return std::make_shared<ConstantExpression<T>>(std::pow(lhs_const->getValue(), rhs_const->getValue()));
                }
                break;
            }
        }
    }
    if (auto expr_mono = dynamic_cast<MonoExpression<T>*>(expr.get())) {
        if (auto val = dynamic_cast<ConstantExpression<T> *>(expr_mono->expr.get())) {
            expr_mono->expr = optimize(expr_mono->expr);
            switch (expr_mono->function) {
                case sin_func:
                    return std::make_shared<ConstantExpression<T>>(std::sin(val->getValue()));
                case cos_func:
                    return std::make_shared<ConstantExpression<T>>(std::cos(val->getValue()));
                case exp_func:
                    return std::make_shared<ConstantExpression<T>>(std::exp(val->getValue()));
                case ln_func:
                    return std::make_shared<ConstantExpression<T>>(std::log(val->getValue()));
            }
        }
    }
    return expr;
}

template <typename T>
T ConstantExpression<T>::eval(std::map<std::string, double> &params) {
    return value;
}

template <typename T>
std::shared_ptr<Expression<T>> ConstantExpression<T>::diff(std::string &s) {
    return std::make_shared<ConstantExpression<T>>(ConstantExpression<T>(0));
}

template <typename T>
std::string ConstantExpression<T>::toString() {
    std::ostringstream oss;
    oss << value;  
    return oss.str();

}

template <typename T>
T VarExpression<T>::eval(std::map<std::string, double> &params) {
    return params[value]; 
}

template <typename T>
std::shared_ptr<Expression<T>> VarExpression<T>::diff(std::string &s) {
    if (s == value) {
        return std::make_shared<ConstantExpression<T>>(ConstantExpression<T>(T(1)));
    }
    else {
        return std::make_shared<ConstantExpression<T>>(T(0));
    }
}

template <typename T>
std::string VarExpression<T>::toString() {
    return value;
}

template <typename T>
T MonoExpression<T>::eval(std::map<std::string, double> &params)  {
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

template <typename T>
std::shared_ptr<Expression<T>> MonoExpression<T>::diff(std::string &s)   {
        auto exp_diff = expr->diff(s);
        switch (function) {
            case cos_func: {
                return std::make_shared<BinaryExpression<T>>(
                    std::make_shared<BinaryExpression<T>>(BinaryExpression<T>(
                        std::make_shared<ConstantExpression<T>>(ConstantExpression<T>(T(-1.0))),
                        std::make_shared<MonoExpression<T>>(MonoExpression<T>(expr, sin_func)),
                    mul_op)),
                    exp_diff,
                    mul_op);
            }
            case sin_func: {
                return std::make_shared<BinaryExpression<T>>(
                    std::make_shared<MonoExpression<T>>(MonoExpression<T>(expr, cos_func)),
                    exp_diff,
                    mul_op);
            }
            case ln_func: {
                return std::make_shared<BinaryExpression<T>>(BinaryExpression<T>(
                    exp_diff,
                    expr,
                    div_op));
            }
            case exp_func: {
                return std::make_shared<BinaryExpression<T>>(BinaryExpression<T>(
                    exp_diff,
                    std::make_shared<MonoExpression<T>>(MonoExpression<T>(expr, function)),
                    mul_op));
            }
        }
        return nullptr;
    }

template <typename T>
std::string MonoExpression<T>::toString()  {
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

template <typename T>
T BinaryExpression<T>::eval(std::map<std::string, double> &params)  {
        switch (op.value) {
            case plus_op:
                if (lhs->eval(params) == T(0) && rhs->eval(params) == T(0)) {
                    return T(0);
                }
            if (lhs->eval(params) == T(0)) {
                return rhs->eval(params);
            }
            if (rhs->eval(params) == T(0)) {
                return lhs->eval(params);
            }
            return lhs->eval(params) + rhs->eval(params);
            case minus_op:
                if (lhs->eval(params) == T(0) && rhs->eval(params) == T(0)) {
                    return T(0);
                }
            if (lhs->eval(params) == T(0)) {
                return rhs->eval(params) * T(-1);
            }
            if (rhs->eval(params) == T(0)) {
                return lhs->eval(params);
            }
            return lhs->eval(params) - rhs->eval(params);
            case mul_op:
                if (lhs->eval(params) == T(0) || rhs->eval(params) == T(0)) {
                    return T(0);
                }
            return lhs->eval(params) * rhs->eval(params);
            case div_op:
                if (rhs->eval(params) == T(0)) {
                    throw ZeroDivExeption();
                }
            if (lhs->eval(params) == T(0)) {
                return T(0);
            }
            return lhs->eval(params) / rhs->eval(params);
            case exp_op:
                if (lhs->eval(params) == T(0) && rhs->eval(params) == T(0)) {
                    throw ZeroDivExeption();
                }
            if (rhs->eval(params) == T(0)) {
                return 1;
            }
            if (rhs->eval(params) == T(1)) {
                return lhs->eval(params);
            }
            return std::pow(lhs->eval(params), rhs->eval(params));
        }
    }

template <typename T>
std::shared_ptr<Expression<T>> BinaryExpression<T>::diff(std::string &s)  {
        auto lhs_diff = lhs->diff(s);
        auto rhs_diff = rhs->diff(s);
        auto lhs_const = dynamic_cast<ConstantExpression<T>*>(lhs.get());
        auto rhs_const = dynamic_cast<ConstantExpression<T>*>(rhs.get());
        auto lhs_diff_const = dynamic_cast<ConstantExpression<T>*>(lhs_diff.get());
        auto rhs_diff_const = dynamic_cast<ConstantExpression<T>*>(rhs_diff.get());
        switch (op.value) {
            case plus_op: {
                if (lhs_diff_const && rhs_diff_const && lhs_diff_const->getValue() == T(0)
                    && rhs_diff_const->getValue() == T(0)) {
                    return std::make_shared<ConstantExpression<T>>(T(0));
                }
                if (lhs_diff_const && lhs_diff_const->getValue() == T(0)) {
                    return rhs_diff;
                }
                if (rhs_diff_const && rhs_diff_const->getValue() == T(0)) {
                    return lhs_diff;
                }
                return std::make_shared<BinaryExpression<T>>(lhs_diff, rhs_diff, plus_op);
            }
            case minus_op: {
                if (lhs_diff_const && rhs_diff_const && lhs_diff_const->getValue() == T(0)
                    && rhs_diff_const->getValue() == T(0)) {
                    return std::make_shared<ConstantExpression<T>>(T(0));
                }
                if (lhs_diff_const && lhs_diff_const->getValue() == T(0)) {
                    return std::make_shared<BinaryExpression<T>>(
                        std::make_shared<ConstantExpression<T>>(T(-1)), rhs_diff, mul_op);
                }
                if (rhs_diff_const && rhs_diff_const->getValue() == T(0)) {
                    return lhs_diff;
                }
                return std::make_shared<BinaryExpression<T>>(lhs_diff, rhs_diff, minus_op);
            }
            case mul_op: {
                if (lhs_const && rhs_const && lhs_const->getValue() == T(0) && rhs_const->getValue() == T(0)) {
                    return std::make_shared<ConstantExpression<T>>(T(0));
                }
                if (lhs_const && lhs_const->getValue() == T(0)) {
                    return std::make_shared<ConstantExpression<T>>(T(0));
                }
                if (rhs_const && rhs_const->getValue() == T(0)) {
                    return std::make_shared<ConstantExpression<T>>(T(0));
                }
                if (lhs_diff_const && rhs_diff_const && lhs_diff_const->getValue() == T(0)
                    && rhs_diff_const->getValue() == T(0)) {
                    return std::make_shared<ConstantExpression<T>>(T(0));
                }
                if (lhs_diff_const && lhs_diff_const->getValue() == T(0)) {
                    return std::make_shared<BinaryExpression<T>>(lhs, rhs_diff, mul_op);
                }
                if (rhs_diff_const && rhs_diff_const->getValue() == T(0)) {
                    return std::make_shared<BinaryExpression<T>>(lhs_diff, rhs, mul_op);
                }
                return std::make_shared<BinaryExpression<T>>(
                    std::make_shared<BinaryExpression<T>>(lhs_diff, rhs, mul_op),
                    std::make_shared<BinaryExpression<T>>(lhs, rhs_diff, mul_op),
                    plus_op);
            }
            case div_op: {
                if (rhs_const && rhs_const->getValue() == T(0)) {
                    throw ZeroDivExeption();
                }
                auto t = std::make_shared<BinaryExpression<T>>(lhs, rhs, mul_op);
                auto t_diff = t->diff(s);
                return std::make_shared<BinaryExpression<T>>(t_diff,
                    std::make_shared<BinaryExpression<T>>(rhs, std::make_shared<ConstantExpression<T>>(T(2)), exp_op),
                    mul_op);
            }
            case exp_op: {
                if (lhs_const && lhs_const->getValue() == T(0)) {
                    throw ZeroExpExeption();
                }
                return std::make_shared<BinaryExpression<T>>(
                    std::make_shared<BinaryExpression<T>>(
                        lhs,
                        rhs,
                        exp_op
                    ),
                    std::make_shared<BinaryExpression<T>>(
                        std::make_shared<BinaryExpression<T>>(
                            rhs_diff,
                            std::make_shared<MonoExpression<T>>(
                                rhs,
                                ln_func
                            ),
                            mul_op),
                        std::make_shared<BinaryExpression<T>>(
                            rhs,
                            std::make_shared<BinaryExpression<T>>(
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

template <typename T>
std::string BinaryExpression<T>::toString() {std::string tmp;
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
#endif //EXPRESSION_H
