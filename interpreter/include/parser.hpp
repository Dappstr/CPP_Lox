
#ifndef PARSER_HPP
#define PARSER_HPP

#pragma once

#include "token.hpp"
#include "expr.hpp"

class Parser {
    private:
        std::vector<Token> m_tokens;
        size_t m_pos {};

        std::shared_ptr<Expr> expression();
        std::shared_ptr<Expr> equality();
        std::shared_ptr<Expr> comparison();
        std::shared_ptr<Expr> term();
        std::shared_ptr<Expr> factor();
        std::shared_ptr<Expr> unary();
        std::shared_ptr<Expr> primary();

        template<typename ... Types>
        bool match(Types ...types);

        bool check(const TokenType& type) const;
        const Token& advance();
        [[nodiscard]] bool is_at_end() const;
        const Token& peek() const;
        const Token& previous() const;

    public:
        explicit Parser(std::vector<Token>&& tokens)
            :m_tokens(std::move(tokens)) {}
        std::shared_ptr<Expr> parse();
};

#endif //PARSER_HPP
