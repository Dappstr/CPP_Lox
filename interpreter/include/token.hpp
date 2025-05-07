#ifndef TOKEN_HPP
#define TOKEN_HPP

#pragma once

#include <iostream>
#include <string>
#include <variant>
#include <optional>

using Literal = std::variant<double, std::string, bool>;
using OptionalLiteral = std::optional<Literal>;

enum class TokenType {
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, COLON,
    SEMICOLON, SLASH, STAR,

    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,

    IDENTIFIER, STRING, NUMBER,

    AND, CLASS, ELSE, FALSE, FUN, FOR,
    IF, NIL, OR, PRINT, RETURN, SUPER,
    THIS, TRUE, VAR, WHILE,

    EndOfFile
};

class Token {
    private:
        TokenType m_type {};
        std::string m_lexeme {};
        OptionalLiteral m_literal {};
        size_t m_line {};

    public:
        Token(TokenType type, const std::string& lexeme, const OptionalLiteral& literal, const size_t line) {
            m_type = type;
            m_lexeme = lexeme;
            m_literal = literal;
            m_line = line;
        }

        friend std::ostream& operator << (std::ostream& out, const Token &token) {
            out << "Type: " << static_cast<int>(token.m_type) << " Lexeme: " << token.m_lexeme << " Line: " << token.m_line;
            if (token.m_literal.has_value()) {
                if (std::holds_alternative<double>(*token.m_literal)) {
                    out << ", Value: " << std::get<double>(*token.m_literal) << ' ';
                } else if (std::holds_alternative<std::string>(*token.m_literal)) {
                    out << ", Value: " << std::get<std::string>(*token.m_literal) << ' ';
                } else {
                    out << ", Value: " << std::boolalpha << std::get<bool>(*token.m_literal) << ' ';
                }
            }
            out << '\n';
            return out;
        }
};

#endif //TOKEN_HPP
