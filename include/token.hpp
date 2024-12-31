#pragma once

#include <optional>
#include <utility>
#include <variant>
#include <string>
#include <iostream>

enum Token_Type {
    // Single-character tokens
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // One or two character tokens
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals
    IDENTIFIER,
    STRING,
    NUMBER,

    // Keywords
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,

    EndOfFile,
};

using Lit = std::optional<std::variant<std::string, double>>;

class Token {
    private:
        Token_Type m_type{};
        std::string m_lexeme{};
        Lit m_literal{};
        size_t m_line{};

    public:
        Token(const Token_Type type, std::string&&  lexeme, Lit&& literal, const size_t line)
            :m_type(type), m_lexeme(std::move(lexeme)), m_literal(std::move(literal)), m_line(line) {}

        friend std::ostream& operator<<(std::ostream& out, const Token& t) {
            out << "Token { type:" << t.m_type << ", lexeme: " << t.m_lexeme << ", literal: ";
            
            if(!t.m_literal.has_value()) {
                out << "null";
            }
            else if(const auto str = std::get_if<std::string>(&t.m_literal.value())) {
                out << *str;
            }
            else if (const auto f = std::get_if<double>(&t.m_literal.value())) {
                out << *f;
            }

            out << ", line: " << t.m_line << "}\n";
            return out;
        }    
};