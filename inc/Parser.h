#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include "Expression.h"

class InvalidOperationException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid syntax";
    }
};

std::pair<std::shared_ptr<Expression>, Token*> parseExpression(Token* token);

#endif //PARSER_H
