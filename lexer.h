//
// Created by Mario on 2020/6/29.
//

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "token.h"


class Lexer {
    inline static std::unordered_map<std::string, TokenType> keywords = {
            {"and",AND},
            {"class",CLASS},
            {"else",ELSE},
            {"false",FALSE},
            {"for",FOR},
            {"func",FUNC},
            {"if",IF},
            {"nil",NIL},
            {"or",OR},
            {"print",PRINT},
            {"return",RETURN},
            {"super",SUPER},
            {"this",THIS},
            {"true",TRUE},
            {"var",VAR},
            {"while",WHILE}
    };

public:
    void input(std::string& source) {
        this->source = source;
        resetState();
        do_lex();
        if (this->hadError) {
            exit(-1);
        }
    }

    void print() {
        for (auto t : this->tokens) {
            printf("%s\n", t.toString().c_str());
        }
    }
private:
    std::string source;
    std::vector<Token> tokens;
    bool hadError;

    uint64_t start = 0;
    uint64_t current = 0;
    uint64_t line = 1;

    void resetState() {
        this->start = 0;
        this->current = 0;
        this->line = 1;
        this->tokens.clear();
        this->hadError = false;
    }

    void do_lex() {
        /*
         * we scan at most 1 token each time
         */
        while(!isAtEnd()) {
            start = current;
            scan();
        }
    }

    void scan() {
        auto ch = advance();
        switch (ch) {
            case '(': addToken(LEFT_PAREN); break;
            case ')': addToken(RIGHT_PAREN); break;
            case '{': addToken(LEFT_BRACE); break;
            case '}': addToken(RIGHT_BRACE); break;
            case ',': addToken(COMMA); break;
            case '.': addToken(DOT); break;
            case '-': addToken(MINUS); break;
            case '+': addToken(PLUS); break;
            case ';': addToken(SEMICOLON); break;
            case '*': addToken(STAR); break;

            case '!': addToken(match('=') ? BANG_EQUAL : BANG); break;
            case '=': addToken(match('=') ? EQUAL_EQUAL : EQUAL); break;
            case '<': addToken(match('=') ? LESS_EQUAL : LESS); break;
            case '>': addToken(match('=') ? GREATER_EQUAL : GREATER); break;

            case '/': {
                if (match('/')) {
                    while(peek() != '\n' && !isAtEnd()) {
                        advance();
                    }
                }else {
                    addToken(SLASH);
                }
            }

            case ' ':
            case '\r':
            case '\t':
                // Ignore whitespace.
                break;

            case '\n':
                line++;
                break;

            case '"': string(); break;

            default: {
                if (isDigit(ch)) {
                    number();
                } else if (isAlpha(ch)) {
                    identifier();
                } else {
                    auto msg = std::string("Unexpected character: ");
                    msg.push_back(ch);
                    this->error(line, msg);
                }
                break;
            }
        }
    }

    void addToken(TokenType type) {
        auto token_str = source.substr(start, current - start);
        tokens.emplace_back(Token(type, token_str, std::any(), line));
    }

    void addToken(TokenType type, std::any&& literal) {
        auto token_str = source.substr(start, current - start);
        tokens.emplace_back(Token(type, token_str, literal, line));
    }

    char advance() {
        return this->source[this->current++];
    }

    // look ahead the next char
    char peek() {
        if (isAtEnd()) return '\0';
        return source.at(current);
    }

    char peeknext() {
        if (current + 1 >= source.length()) return '\0';
        return source.at(current + 1);
    }

    /*
     * the last valid index of source is source.length() - 1
     * if current >= source.length(), the lex is ended
     */
    bool isAtEnd() {
        return current >= source.length();
    }

    bool isAlpha(char c) {
        return (c >= 'a' && c <= 'z') ||
               (c >= 'A' && c <= 'Z') ||
               c == '_';
    }

    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    bool isAlphaNumeric(char c) {
        return isAlpha(c) || isDigit(c);
    }

    /*
     * check if the current char === expected
     * if true, advance and return true
     */
    bool match(char expected) {
        if (isAtEnd()) return false;
        if (peek() != expected) return false;
        advance();
        return true;
    }

    void identifier() {
        while (isAlphaNumeric(peek())) advance();
        auto id_str = this->source.substr(start, current - start);
        auto it = keywords.find(id_str);
        if (it == keywords.end())
            addToken(IDENTIFIER);
        else
            addToken(it->second);
    }

    void number() {
        while(isdigit(peek())) advance();
        if (peek() == '.' && isnumber(peeknext())) {
            advance();
            while (isDigit(peek())) advance();
        }
        addToken(NUMBER, std::stod(source.substr(start, current - start)));
    }

    void string() {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n') line++;
            advance();
        }

        // Unterminated string.
        if (isAtEnd()) {
            this->error(line, "Unterminated string.");
            return;
        }

        // The closing ".
        advance();

        // Trim the surrounding quotes.
        auto value = source.substr(start + 1, current - start - 2);
        addToken(STRING, value);
    }

    void report(int line, std::string where, std::string message) {
        auto msg = "[line " + std::to_string(line) + "] Error" + where + ": " + message;
        printf("%s\n", msg.c_str());
        hadError = true;
    }

    void error(int line, std::string message) {
        report(line, "", message);
    }
};


