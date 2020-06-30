//
// Created by Mario on 2020/6/30.
//

#include "expression.h"
#include "expression_visitor.h"

std::string Binary::accept(ExprVisitor* visitor) {
    return visitor->visit(this);
}

std::string Unary::accept(ExprVisitor* visitor) {
    return visitor->visit(this);
}

std::string Grouping::accept(ExprVisitor* visitor) {
    return visitor->visit(this);
}

std::string Literal::accept(ExprVisitor* visitor) {
    return visitor->visit(this);
}
