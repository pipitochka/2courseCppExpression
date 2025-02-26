#include "../inc/Parser.h"

enum STATE {N, O};

std::pair<std::shared_ptr<Expression>, Token*> parseExpression(Token* token, int rank, bool flag) {
    STATE state = N;
    std::shared_ptr<Expression> tmp;
    while (token != nullptr) {
        switch (state) {
            case N:
                switch (token->type) {
                    case OP: {
                        throw InvalidOperationException();
                    }
                    case NUM: {
                        tmp = std::make_shared<ConstantExpression>(std::stod(token->value));
                        state = O;
                        token = token->next.get();
                        break;
                    }
                    case ID: {
                        tmp = std::make_shared<VarExpression>(token->value);
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
                            auto var = parseExpression(token->next.get(), 0, true);
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
                        auto var = parseExpression(token->next.get(), 0, true);
                        token = var.second;
                        tmp = std::make_shared<MonoExpression>(var.first, f);
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
                        if (q.rank < rank) {
                            return {tmp, token};
                        }
                        auto var = parseExpression(token->next.get(), q.rank, false);
                        token = var.second;
                        Expression* tok = var.first.get();
                        // if (dynamic_cast<BinaryExpression*>(tok) != nullptr) {
                        //     while (true) {
                        //         auto x = static_cast<BinaryExpression*>(tok);
                        //         BinaryExpression* y = nullptr;
                        //         while (x->getOp().rank < q.rank) {
                        //             if (dynamic_cast<BinaryExpression*>(x->getLhs().get()) == nullptr) {
                        //                 break;
                        //             }
                        //             y = x;
                        //             x = static_cast<BinaryExpression*>(x->getLhs().get());
                        //         };
                        //         if (y != nullptr) {
                        //             y->setLhs(std::make_shared<BinaryExpression>(tmp, y->getLhs(), op));
                        //             tmp = var.first;
                        //             break;
                        //         }
                        //         else {
                        //             tmp = std::make_shared<BinaryExpression>(tmp, var.first, op);
                        //             break;
                        //         }
                        //     }
                        // }
                        // else {
                            tmp = std::make_shared<BinaryExpression>(tmp, var.first, op);
                        
                        break;
                    }
                }
        }
    }
    return std::make_pair(tmp, nullptr);
}

std::pair<std::shared_ptr<Expression>, Token*> parseExpression(Token* token) {
    return parseExpression(token, 0, false);
}