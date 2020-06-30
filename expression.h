//
// Created by Mario on 2020/6/30.
//

#pragma once
#include "token.h"
#include <vector>

class ExprVisitor;
class Expr {
public:
    virtual std::string accept(ExprVisitor* visitor) = 0;
};

struct Binary : public Expr {
    Binary(Expr* left, Token* token, Expr* right) {
        this->left = left;
        this->token = token;
        this->right = right;
    }

    virtual std::string accept(ExprVisitor* visitor) final;

    Expr* left;
    Token* token;
    Expr* right;
};

struct Unary : public Expr {
    Unary(Token* token, Expr* right) {
        this->token = token;
        this->right = right;
    }

    virtual std::string accept(ExprVisitor* visitor) final;

    Token* token;
    Expr* right;
};

struct Grouping : public Expr {
    Grouping(Expr* expr) {
        this->expr = expr;
    }

    virtual std::string accept(ExprVisitor* visitor) final;

    Expr* expr;
};

struct Literal : public Expr {
    Literal(double literal) {
        this->literal = literal;
    }

    virtual std::string accept(ExprVisitor* visitor) final;

    double literal;
};

