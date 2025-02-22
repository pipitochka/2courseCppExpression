#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <map>
#include <string>
#include <functional>
#include <utility>

enum operators {plus_op, minus_op, div_op, mul_op, exp_op};
enum functions {sin_func, cos_func, ln_func, exp_func};

class ZeroDivExeption : public std::exception {
    public:
    const char* what() const noexcept override {
        return "Division by zero!";
    }
};


class Expression {
public:
    virtual std::shared_ptr<Expression> diff() = 0;
    virtual double eval(std::map<std::string, int> &params) = 0;
};

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
    
    double eval(std::map<std::string, int> &params) override;

    std::shared_ptr<Expression> diff() override;
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

    double eval(std::map<std::string, int> &params) override;

    std::shared_ptr<Expression> diff() override;
};

class BinaryExpression;

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

    double eval(std::map<std::string, int> &params) override;

    std::shared_ptr<Expression> diff() override;
};

class BinaryExpression : public Expression {
    private:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
    operators op;
    
    public:
    BinaryExpression(const std::shared_ptr<Expression> &lhs, const std::shared_ptr<Expression> &rhs, operators op) {
        this->lhs = lhs;
        this->rhs = rhs;
        this->op = op;
    }
    ~BinaryExpression() = default;
    BinaryExpression(const BinaryExpression&) = default;
    BinaryExpression& operator=(const BinaryExpression&) = default;
    BinaryExpression(BinaryExpression&&) = default;
    BinaryExpression& operator=(BinaryExpression&&) = default;

    double eval(std::map<std::string, int> &params) override;

    std::shared_ptr<Expression> diff() override;
};




#endif //EXPRESSION_H
