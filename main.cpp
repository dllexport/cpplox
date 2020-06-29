#include <iostream>
#include "lexer.h"

int main() {
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
