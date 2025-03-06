#pragma once
#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include "Expression.h"

enum STATE {N, O};

class InvalidOperationException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid syntax";
    }
};

template <typename T>
std::pair<std::shared_ptr<Expression<T>>, Token*> parseExpression(Token* token, int rank, bool flag) {
    STATE state = N;
    std::shared_ptr<Expression<T>> tmp;
    while (token != nullptr) {
        switch (state) {
            case N:
                switch (token->type) {
                    case OP: {
                        throw InvalidOperationException();
                    }
                    case NUM: {
                        tmp = std::make_shared<ConstantExpression<T>>(std::stod(token->value));
                        state = O;
                        token = token->next.get();
                        break;
                    }
                    case ID: {
                        tmp = std::make_shared<VarExpression<T>>(token->value);
                        state = O;
                        token = token->next.get();
                        break;
                    }
                    case SCOPE: {
                        if (token->value == ")") {
                            if (flag) {
                                return {tmp, token->next.get()};
                            }
                            else {
                                return {tmp, token};
                            }
                        }
                        if (token->value == "(") {
                            auto var = parseExpression<T>(token->next.get(), 0, true);
                            token = var.second;
                            tmp = var.first;
                            state = O;
                            break;
                        }
                    }
                    case FUNC: {
                        functions f = token->value == "sin" ? sin_func : token->value == "cos" ? cos_func : token->value == "exp" ? exp_func : ln_func;
                        token = token->next.get();
                        if (token == nullptr || token->value != "(") {
                            throw InvalidOperationException();
                        }
                        auto var = parseExpression<T>(token->next.get(), 0, true);
                        token = var.second;
                        tmp = std::make_shared<MonoExpression<T>>(var.first, f);
                        state = O;
                        break;
                    }
                }
                break;
            case O:
                switch (token->type) {
                    case NUM:
                    case ID:
                    case FUNC:
                        throw InvalidOperationException();
                    case SCOPE: {
                        if (token->value == ")") {
                            if (flag) {
                                return {tmp, token->next.get()};
                            }
                            else {
                                return {tmp, token};
                            }                        }
                        if (token->value == "(") {
                            throw InvalidOperationException();
                        }
                    }
                    case OP: {
                        oper op = token->value == "+" ? plus_op : token->value == "-" ? minus_op : token->value == "*" ? mul_op : token->value == "/" ? div_op : exp_op;
                        operators q = operators(op);
                        if (q.rank <= rank) {
                            return {tmp, token};
                        }
                        auto var = parseExpression<T>(token->next.get(), q.rank, false);
                        token = var.second;
                        tmp = std::make_shared<BinaryExpression<T>>(tmp, var.first, op);
                        
                        break;
                    }
                }
        }
    }
    return std::make_pair(tmp, nullptr);
}

template <typename T>
std::shared_ptr<Expression<T>> parseExpression(Token* token) {
    return parseExpression<T>(token, 0, false).first;
}

#endif //PARSER_H
