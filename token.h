//
// Created by Mario on 2020/6/29.
//

#pragma once
#include "token_type.h"
#include <string>
#include <any>

class Token {
public:
    Token(TokenType type,
          std::string lexeme,
          std::any literal,
          uint32_t line) : type(type), lexeme(lexeme), literal(literal), line(line) {}

    std::string Lexeme() {
        return this->lexeme;
    }
    std::string toString() {
        static char buff[1024];
        sprintf(buff,"type: [%s], lexeme: [%s], literal: [%s]", std::to_string(type).c_str(), lexeme.c_str(), literal_to_string().c_str());
        return std::string(buff);
    }

    TokenType Type() {
        return this->type;
    }

    std::any Literal() {
        return literal;
    }

private:
    TokenType type;
    std::string lexeme;
    std::any literal;
    uint32_t line;

    std::string literal_to_string() {
        switch (type) {
            case NUMBER: {
                return std::to_string(std::any_cast<double>(literal));
            }
            default: {
                if (this->literal.has_value()) {
                    return std::any_cast<std::string>(literal);
                }else {
                    return "nil";
                }
            }
        }
    }
};


