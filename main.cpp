#include <iostream>
#include "lexer.h"
#include "expression.h"
#include "expression_visitor.h"

int main() {

    auto expression = new Binary(
            new Unary(new Token(MINUS, "-", nullptr, 1), new Literal(123)),
            new Token(STAR, "*", nullptr, 1),
            new Grouping(new Literal(45.67))
            );

    AstPrinter().print(expression);
    char inBuff[1024];
    std::cout << "lox\n";
    Lexer lexer;
    while(1) {
        std::cout << "> ";
        std::cin.getline(inBuff, 1024);
        auto in = std::string(inBuff);
        if (in == "exit") {
            exit(0);
        }
        lexer.input(in);
        lexer.print();
    }
    return 0;
}
