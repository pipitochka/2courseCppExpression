#include "../inc/Token.h"

#include <iostream>
#include <ostream>

std::string data[] = {"sin", "cos", "ln", "exp"};

std::string lower(std::string& str) {
    std::string res(str);
    std::transform(res.begin(), res.end(), res.begin(), ::tolower);
    return res;
}

void checkToken(Token *token) {
    for (auto el : data) {
        if (el == lower(token->value)) {
            token->type = FUNC;
            token->value = lower(token->value);
            return;
        }
    }
}

std::unique_ptr<Token> tokenize(std::string input) {
    auto head = std::make_unique<Token>("", START);
    Token* curr = head.get();
    int i = 0;
    while (i < input.size()) {
        while (i < input.size() && input[i] == ' ') {
            i++;
        }
        if (i >= input.size()) {
            break;
        }
        if (input[i] == '(' || input[i] == ')') {
            curr->next = std::make_unique<Token>(input.substr(i, 1), TokenType::SCOPE);
            curr = curr->next.get();
            i++;
        }
        else if (std::isdigit(input[i])) {
            int j = i;
            while (j < input.size() && std::isdigit(input[j])) {
                j++;
            }
            curr->next = std::make_unique<Token>(input.substr(i, j - i), TokenType::NUM);
            curr = curr->next.get();
            i = j;
        }
        else if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' || input[i] == '^') {
            curr->next = std::make_unique<Token>(input.substr(i, 1), TokenType::OP);
            curr = curr->next.get();
            i++;
        }
        else if (std::isalpha(input[i])) {
            int j = i;
            while (j < input.size() && std::isalnum(input[j])) {
                j++;
            }
            curr->next = std::make_unique<Token>(input.substr(i, j - i), TokenType::ID);
            curr = curr->next.get();
            checkToken(curr);
            i = j;;
        }
        else {
            throw InvalidSymbolException();
        }
    }
    return std::move(head->next);
}

void printToken(const Token *token) {
    if (token == nullptr) {
        return;
    }
    std::cout << token->value << std::endl;
    if (token->next != nullptr) {
        printToken(token->next.get());
    }
}