#include "../include/parser.hpp"
#include "../include/expr.hpp"

#include <stdexcept>

class Binary_Expr;

std::shared_ptr<Expr> Parser::parse() { return expression(); }

template<typename ... Types>
bool Parser::match(Types... types) {
    static_assert((std::is_same_v<Types, TokenType> && ...), "match() only accepts TokenTypes!");
    return ((check(types) ? (advance(), true) : false) || ...);
}

std::shared_ptr<Expr> Parser::expression() {
    return equality();
}

std::shared_ptr<Expr> Parser::equality() {
    auto expr = comparison();
    while (match(TokenType::BANG_EQUAL, TokenType::EQUAL)) {
        Token op = previous();
        auto right = comparison();
        expr = std::make_shared<Binary_Expr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::comparison() {
    auto expr = term();
    while (match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL)) {
        Token op = previous();
        auto right = term();
        expr = std::make_shared<Binary_Expr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::term() {
    auto expr = factor();
    while (match(TokenType::PLUS, TokenType::MINUS)) {
        Token op = previous();
        auto right = factor();
        expr = std::make_shared<Binary_Expr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::factor() {
    auto expr = unary();
    while (match(TokenType::STAR, TokenType::SLASH)) {
        Token op = previous();
        auto right = unary();
        expr = std::make_shared<Binary_Expr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::unary() {
    if (match(TokenType::BANG, TokenType::MINUS)) {
        Token op = previous();
        auto right = unary();
        return std::make_shared<Unary_Expr>(right, op);
    }
    return primary();
}

std::shared_ptr<Expr> Parser::primary() {
    if (match(TokenType::FALSE)) return std::make_shared<Literal_Expr>(false);
    if (match(TokenType::TRUE)) return std::make_shared<Literal_Expr>(true);
    if (match(TokenType::NIL)) return std::make_shared<Literal_Expr>(std::nullopt);

    if (match(TokenType::NUMBER, TokenType::STRING)) {
        return std::make_shared<Literal_Expr>(previous().literal().value());
    }

    if (match(TokenType::LEFT_PAREN)) {
        auto expr = expression();
        if (!match(TokenType::RIGHT_PAREN)) {
            throw std::runtime_error("Expected ')' after expression.");
        }
        return std::make_shared<Grouping_Expr>(std::vector<std::shared_ptr<Expr>>{expr});
    }
    throw std::runtime_error("Unexpected expression.");
}

bool Parser::check(const TokenType &type) const {
    return !is_at_end() && peek().type() == type;
}

const Token & Parser::advance() { if (!is_at_end()) { m_pos += 1;} return previous();}
bool Parser::is_at_end() const { return m_pos >= m_tokens.size(); }
const Token & Parser::peek() const { return m_tokens.at(m_pos); }
const Token & Parser::previous() const { return m_tokens.at(m_pos - 1); }
