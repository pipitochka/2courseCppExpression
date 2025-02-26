#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <map>
#include <string>
#include <set>
#include <memory>
#include <cmath>

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


class Expression {
public:
    virtual std::shared_ptr<Expression> diff(std::string &s) = 0;
    virtual double eval(std::map<std::string, double> &params) = 0;
    virtual std::string toString() = 0;
};


class ConstantExpression;
class VarExpression;
class MonoExpression;
class BinaryExpression;


class ConstantExpression : public Expression {
private:
    double value;
    
public:
    ConstantExpression(double value) : value(value) {};
    ~ConstantExpression() = default;
    ConstantExpression(const ConstantExpression&) = default;
    ConstantExpression& operator=(const ConstantExpression&) = default;
    ConstantExpression(ConstantExpression&&) = default;
    ConstantExpression& operator=(ConstantExpression&&) = default;

    double getValue() {
        return value;
    }
    double eval(std::map<std::string, double> &params) override;
    std::shared_ptr<Expression> diff(std::string &s) override;
    std::string toString() override;
};


class VarExpression : public Expression {
private:
    std::string value;

public:
    VarExpression(const std::string &value) : value(value) {};
    ~VarExpression() = default;
    VarExpression(const VarExpression&) = default;
    VarExpression& operator=(const VarExpression&) = default;
    VarExpression(VarExpression&&) = default;
    VarExpression& operator=(VarExpression&&) = default;

    double eval(std::map<std::string, double> &params) override;
    std::shared_ptr<Expression> diff(std::string &s) override;
    std::string toString() override;
};

class MonoExpression : public Expression {
private:
    std::shared_ptr<Expression> expr;
    functions function;
    
public:
    MonoExpression(const std::shared_ptr<Expression> &expr, functions function) {
        this->expr = expr;
        this->function = function;
    }
    ~MonoExpression() = default;
    MonoExpression(const MonoExpression&) = default;
    MonoExpression& operator=(const MonoExpression&) = default;
    MonoExpression(MonoExpression&&) = default;
    MonoExpression& operator=(MonoExpression&&) = default;

    double eval(std::map<std::string, double> &params) override;
    std::shared_ptr<Expression> diff(std::string &s) override;
    std::string toString() override;
    friend std::shared_ptr<Expression> optimize(std::shared_ptr<Expression> expr);
};

class BinaryExpression : public Expression {
private:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
    operators op;
    
public:
    BinaryExpression(const std::shared_ptr<Expression> &lhs, const std::shared_ptr<Expression> &rhs, oper op): op(op) {
        this->lhs = lhs;
        this->rhs = rhs;
    }

    ~BinaryExpression() = default;
    BinaryExpression(const BinaryExpression&) = default;
    BinaryExpression& operator=(const BinaryExpression&) = default;
    BinaryExpression(BinaryExpression&&) = default;
    BinaryExpression& operator=(BinaryExpression&&) = default;

    std::shared_ptr<Expression> getLhs() { return lhs; }
    void setLhs(std::shared_ptr<Expression> lhs) { this->lhs = lhs; }
    operators getOp() { return op; }
    double eval(std::map<std::string, double> &params) override;
    std::shared_ptr<Expression> diff(std::string &s) override;
    std::string toString() override;
    friend std::shared_ptr<Expression> optimize(std::shared_ptr<Expression> expr);
};

std::shared_ptr<Expression> optimize(std::shared_ptr<Expression> expr);

#endif //EXPRESSION_H
