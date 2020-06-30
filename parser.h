//
// Created by Mario on 2020/6/30.
//

#pragma once
#include <vector>
#include "token.h"
#include "expression.h"

class Parser {
public:
    Parser(std::vector<Token>& tokens) : tokens(tokens) {}
private:
    int current = 0;
    std::vector<Token> tokens;

    Expr* expression() {
        return equality();
    }

    Expr* equality() {
        Expr* expr = comparison();

        while (match({BANG_EQUAL, EQUAL_EQUAL})) {
            Token* token = previous();
            Expr* right = comparison();
            expr = new Binary(expr, token, right);
        }

        return expr;
    }

    Expr* comparison() {
        Expr* expr = addition();

        while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
            Token* token = previous();
            Expr* right = addition();
            expr = new Binary(expr, token, right);
        }

        return expr;
    }

    Expr* addition() {
        Expr* expr = multiplication();

        while (match({MINUS, PLUS})) {
            Token* token = previous();
            Expr* right = multiplication();
            expr = new Binary(expr, token, right);
        }

        return expr;
    }


    Expr* multiplication() {
        Expr* expr = unary();

        while (match({SLASH, STAR})) {
            Token* token = previous();
            Expr* right = unary();
            expr = new Binary(expr, token, right);
        }

        return expr;
    }

    Expr* unary() {
        if (match({BANG, MINUS})) {
            Token* token = previous();
            Expr* right = unary();
            return new Unary(token, right);
        }

        return primary();
    }

    Expr* primary() {
        if (match({FALSE})) return new Literal(false);
        if (match({TRUE})) return new Literal(true);
        if (match({NIL})) return new Literal(std::any());

        if (match({NUMBER, STRING})) {
            return new Literal(previous()->Literal());
        }

        if (match({LEFT_PAREN})) {
            Expr* expr = expression();
            consume(RIGHT_PAREN, "Expect ')' after expression.");
            return new Grouping(expr);
        }
    }
    Token* consume(TokenType type, std::string message) {
        if (check(type)) return advance();

        printf("consume error\n");
//        throw error(peek(), message);
    }

    bool match(std::vector<TokenType> types) {
        for (auto type : types) {
            if (check(type)) {
                advance();
                return true;
            }
        }

        return false;
    }

    bool check(TokenType type) {
        if (isAtEnd()) return false;
        return peek()->Type() == type;
    }

    Token* advance() {
        if (!isAtEnd()) current++;
        return previous();
    }

    bool isAtEnd() {
        return peek()->Type() == EOF;
    }

    Token* peek() {
        return &tokens.at(current);
    }

    Token* previous() {
        return &tokens.at(current - 1);
    }
};


