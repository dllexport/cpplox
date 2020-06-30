//
// Created by Mario on 2020/6/30.
//

#pragma once
#include "expression_visitor.h"

class ExprVisitor {
public:
    virtual std::string visit(Binary* expr) = 0;
    virtual std::string visit(Unary* expr) = 0;
    virtual std::string visit(Grouping* expr) = 0;
    virtual std::string visit(Literal* expr) = 0;
};
#include <iostream>
class AstPrinter : public ExprVisitor {
public:
    void print(Expr* expr) {
        std::cout << expr->accept(this);
    }

    std::string parenthesize(std::string name, std::vector<Expr*> exprs) {

        std::string res;
        res.push_back('(');
        res.append(name);
        for (auto expr : exprs) {
            res.append(" ");
            res.append(expr->accept(this));
        }
        res.push_back(')');
        return res;
    }

    virtual std::string visit(Binary* expr) final {
        return parenthesize(expr->token->Lexeme(), {expr->left, expr->right});
    }
    virtual std::string visit(Unary* expr) final {
        return parenthesize(expr->token->Lexeme(), {expr->right});
    }
    virtual std::string visit(Grouping* expr) final {
        return parenthesize("grouping", {expr->expr});
    }
    virtual std::string visit(Literal* expr) final {
        return std::to_string(expr->literal);
//        parenthesize("Literal", expr->literal);
    }
};