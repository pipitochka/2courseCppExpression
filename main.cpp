#include <iostream>
#include "inc/Expression.h"
#include "inc/Token.h"
#include "inc/Parser.h"

class InvalidSyntaxException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid syntax";
    }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        throw InvalidSyntaxException();
    }
    std::string command = argv[1];
    std::string value = argv[2];

    if (command == "--eval") {
        if (argc <= 2) {
            throw InvalidSyntaxException();
        }
        std::map<std::string, double> env;
        for (int i = 3; i < argc; i++) {
            std::string data = argv[i];
            auto t = data.find('=');
            std::string key = data.substr(0, t);
            double value_key = std::stod(data.substr(t + 1));
            env.insert(std::pair<std::string, double>(key, value_key));
        }
        auto token = tokenize(value);
        auto expr = parseExpression<double>(token.get());
        std::cout << expr->eval(env) << std::endl;
    }

    else if (command == "--diff") {
        if (argc != 5) {
            throw InvalidSyntaxException();
        }
        std::string command2 = argv[3];
        std::string param = argv[4];
        if (command2 != "--by") {
            throw InvalidSyntaxException();
        }
        auto token = tokenize(value);
        auto expr = parseExpression<double>(token.get());
        std::cout << optimize(expr->diff(param))->toString() << std::endl;
    }
}
