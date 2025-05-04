
#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "token.hpp"
#include <map>

static std::unordered_map<std::string, TokenType> keywords = {
    {"and", TokenType::AND},
    {"class", TokenType::CLASS},
    {"else", TokenType::ELSE},
    {"false", TokenType::FALSE},
    {"for", TokenType::FOR},
    {"fun", TokenType::FUN},
    {"if", TokenType::IF},
    {"nil", TokenType::NIL},
    {"or", TokenType::OR},
    {"print", TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super", TokenType::SUPER},
    {"this", TokenType::THIS},
    {"true", TokenType::TRUE},
    {"var", TokenType::VAR},
    {"while", TokenType::WHILE},
};

class Scanner {
    private:
        std::string m_src;
        std::vector<Token> m_tokens;
        size_t m_start = 0, m_current = 0, m_line = 1;

    public:
        Scanner(std::string&& src) : m_src(std::move(src)) {}
        std::vector<Token> scan();
        [[nodiscard]] bool isAtEnd() const;
        void scanToken();
        char advance();
        [[nodiscard]] char peek() const;
        [[nodiscard]] char peekNext() const;
        bool match(char expected);
        void addNullToken(const TokenType& type);
        void addToken(const TokenType& type, const OptionalLiteral& literal);
        void string();
        void number();
        void identifier();
};

#endif //SCANNER_HPP
