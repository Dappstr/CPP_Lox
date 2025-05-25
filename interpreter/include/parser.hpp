
#ifndef PARSER_HPP
#define PARSER_HPP

#pragma once

#include "token.hpp"
#include "expr.hpp"
#include "statement.hpp"

class Parser {
    private:
        std::vector<Token> m_tokens;
        size_t m_pos {};

        std::shared_ptr<Stmt> declaration();
        std::shared_ptr<Stmt> statement();
        std::shared_ptr<Stmt> print_statement();
        std::shared_ptr<Stmt> expression_statement();
        std::shared_ptr<Stmt> var_declaration();
        std::shared_ptr<Stmt> if_statement();
        std::shared_ptr<Stmt> while_statement();
        std::vector<std::shared_ptr<Stmt>> block_statement();
        void synchronize();

        std::shared_ptr<Expr> expression();
        std::shared_ptr<Expr> assignment();
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
        const Token& consume(TokenType type, const std::string& message);


    public:
        explicit Parser(std::vector<Token>&& tokens)
            :m_tokens(std::move(tokens)) {}
        std::vector<std::shared_ptr<Stmt>> parse();
};

#endif //PARSER_HPP
