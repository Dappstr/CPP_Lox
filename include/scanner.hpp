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
        explicit Scanner(std::string&& source);
        std::vector<Token> scan_tokens();
        void scan_token();
        bool is_at_end() const;
        char advance();
        void add_null_token(Token_Type type);
        void add_token(Token_Type type, Lit&& literal);
        bool match(char expected);
        char peek() const;
        char peek_next() const;
        void str();
        void number();
        void identifier();
};