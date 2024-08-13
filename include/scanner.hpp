#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "token.hpp"

class Scanner {
    private:
        size_t m_start = 0;
        size_t m_current = 0;
        size_t m_line = 1;

        std::string m_source{};
        std::vector<Token> m_tokens{};
        std::unordered_map<std::string, Token_Type> m_keywords = {
            {"and", AND},
            {"class", CLASS},
            {"else", ELSE},
            {"false", FALSE},
            {"for", FOR},
            {"fun", FUN},
            {"if", IF},
            {"nil", NIL},
            {"or", OR},
            {"print", PRINT},
            {"return", RETURN},
            {"super", SUPER},
            {"this", THIS},
            {"true", TRUE},
            {"var", VAR},
            {"while", WHILE},
        };

    public:
        Scanner(const std::string& source);
        std::vector<Token> scan_tokens();
        void scan_token();
        bool is_at_end();
        char advance();
        void add_null_token(const Token_Type type);
        void add_token(const Token_Type type, const Lit& literal);
        bool match(const char expected);
        char peek();
        char peek_next();
        void str();
        void number();
        void identifier();
};