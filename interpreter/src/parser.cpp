#include "../include/parser.hpp"
#include "../include/expr.hpp"
#include "../include/statement.hpp"

#include <stdexcept>
#include <vector>

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!is_at_end()) {
        auto stmt = declaration();
        if (stmt) {
            statements.push_back(stmt);
        }
    }
    return statements;
}

template<typename ... Types>
bool Parser::match(Types... types) {
    static_assert((std::is_same_v<Types, TokenType> && ...), "match() only accepts TokenTypes!");
    return ((check(types) ? (advance(), true) : false) || ...);
}

std::shared_ptr<Stmt> Parser::declaration() {
    try {
        if (match(TokenType::VAR)) return var_declaration();
        return statement();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        synchronize();
        return nullptr;
    }
}

std::shared_ptr<Stmt> Parser::var_declaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expected a variable name.");
    std::shared_ptr<Expr> initializer = nullptr;

    if (match(TokenType::EQUAL)) {
        initializer = expression();
    }
    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration.");
    return std::make_shared<Var_Stmt>(name, initializer);
}

std::shared_ptr<Stmt> Parser::statement() {
    if (match(TokenType::PRINT)) { return print_statement(); }
    return expression_statement();
}

std::shared_ptr<Stmt> Parser::print_statement() {
    std::shared_ptr<Expr> value = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after statement.");
    return std::make_shared<Print_Stmt>(value);
}

std::shared_ptr<Stmt> Parser::expression_statement() {
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression.");
    return std::make_shared<Expression_Stmt>(expr);
}

void Parser::synchronize() {
    advance();
    while (!is_at_end()) {
        if (previous().type() == TokenType::SEMICOLON) return;

        switch (peek().type()) {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
            default:
                break;
        }
        advance();
    }
}

std::shared_ptr<Expr> Parser::expression() {
    return assignment();
}

std::shared_ptr<Expr> Parser::assignment() {
    auto expr = equality();

    if (match(TokenType::EQUAL)) {
        Token equals = previous();
        auto value = assignment();  // right-associative

        if (auto var = std::dynamic_pointer_cast<Variable_Expr>(expr)) {
            return std::make_shared<Assign_Expr>(var->m_name, value);
        }

        throw std::runtime_error("Invalid assignment target.");
    }

    return expr;
}


std::shared_ptr<Expr> Parser::equality() {
    auto expr = comparison();
    while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
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
    if (match(TokenType::FALSE)) {
        return std::make_shared<Literal_Expr>(false);
    }

    if (match(TokenType::TRUE)) {
        return std::make_shared<Literal_Expr>(true);
    }

    if (match(TokenType::NIL)) {
        return std::make_shared<Literal_Expr>(std::nullopt);
    }

    if (match(TokenType::NUMBER, TokenType::STRING)) {
        const Token& tok = previous();
        if (!tok.literal().has_value()) {
            throw std::runtime_error("Literal token has no value.");
        }
        return std::make_shared<Literal_Expr>(tok.literal().value());
    }

    if (match(TokenType::LEFT_PAREN)) {
        std::shared_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");
        return std::make_shared<Grouping_Expr>(std::vector<std::shared_ptr<Expr>>{expr});
    }

    if (match(TokenType::IDENTIFIER)) {
        return std::make_shared<Variable_Expr>(previous());
    }

    if (peek().type() == TokenType::EndOfFile) {
        throw std::runtime_error("Unexpected end of input. Perhaps you forgot a semicolon?");
    }

    throw std::runtime_error("Unexpected expression near: '" + peek().lexeme() + "'");
}

bool Parser::check(const TokenType &type) const {
    return !is_at_end() && peek().type() == type;
}

const Token & Parser::advance() { if (!is_at_end()) { m_pos += 1;} return previous();}
bool Parser::is_at_end() const { return peek().type() == TokenType::EndOfFile; }
const Token & Parser::peek() const { return m_tokens.at(m_pos); }
const Token & Parser::previous() const { return m_tokens.at(m_pos - 1); }
const Token& Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw std::runtime_error("Line " + std::to_string(peek().line()) + ": " + message +
                             " (found '" + peek().lexeme() + "')");
}

