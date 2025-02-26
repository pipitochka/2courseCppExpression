#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType { START, FUNC, NUM, ID, SCOPE, OP};

class InvalidSymbolException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid syntax";
    }
};

class Token {  
public:
    std::unique_ptr<Token> next;
    std::string value;
    TokenType type;
    Token(std::string value, TokenType type) : value(value), type(type) {
        next = nullptr;
    };
};

std::unique_ptr<Token> tokenize(std::string input);

void printToken(const Token* token);

#endif //TOKEN_H
