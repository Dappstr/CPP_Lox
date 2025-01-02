#include "expression.hpp"
#include "parser.hpp"

std::unique_ptr<Expression> Parser::parse() {
	try {
		return expression();
	} catch (const std::runtime_error& e) {
		synchronize();
		std::cout << e.what() << '\n';
		return nullptr;
	}
}

std::unique_ptr<Expression> Parser::expression() { return equality(); }

std::unique_ptr<Expression> Parser::equality() {
	auto expr = comparison();
	while (match(BANG, BANG_EQUAL)) {
		const auto op = previous().type();
		auto right = comparison();
		expr = std::make_unique<Binary_Expression>(std::move(expr), op, std::move(right));
	}
	return std::move(expr);
}

std::unique_ptr<Expression> Parser::comparison() {
	auto expr = term();
	while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
		const auto op = previous().type();
		auto right = term();
		expr = std::make_unique<Binary_Expression>(std::move(expr), op, std::move(right));
	}
	return std::move(expr);
}

std::unique_ptr<Expression> Parser::term() {
	auto expr = factor();
	while (match(MINUS, PLUS)) {
		const auto op = previous().type();
		auto right = factor();
		expr = std::make_unique<Binary_Expression>(std::move(expr), op, std::move(right));
	}
	return std::move(expr);
}

std::unique_ptr<Expression> Parser::factor() {
	auto expr = unary();
	while (match(SLASH, STAR)) {
		const auto op = previous().type();
		auto right = unary();
		expr = std::make_unique<Binary_Expression>(std::move(expr), op, std::move(right));
	}
	return std::move(expr);
}

std::unique_ptr<Expression> Parser::unary() {
	if (match(BANG, MINUS)) {
		const auto op = previous().type();
		auto right = unary();
		return std::make_unique<Unary_Expression>(op, std::move(right));
	}
	return primary();
}

std::unique_ptr<Expression> Parser::primary() {
	if (match(FALSE)) return std::make_unique<Literal_Expression>(false);
	if (match(TRUE)) return std::make_unique<Literal_Expression>(true);
	if (match(NIL)) return std::make_unique<Literal_Expression>(nullptr);

	if (match(NUMBER, STRING)) { return std::make_unique<Literal_Expression>(previous().literal()); }

	if (match(LEFT_PAREN)) {
		auto expr = expression(); // Go back up to the start of parsing an expression
		consume(RIGHT_PAREN, "Expected \')\' after expression.");
		return expr;
	}
	throw std::runtime_error("Unexpected expression.");
}

bool Parser::check(const Token_Type type) {
	if (is_at_end()) { return false; }
	return peek().type() == type;
}

Token& Parser::advance() {
	if (!is_at_end()) { m_current++; }
	return previous();
}

bool Parser::is_at_end() { return peek().type() == END_OF_FILE; }

Token& Parser::peek() { return m_tokens.at(m_current); }

Token& Parser::previous() { return m_tokens.at(m_current - 1); }

Token& Parser::consume(const Token_Type type, const char* message) {
	if (check(type)) { return advance(); }
	throw std::runtime_error(message);
}

void Parser::synchronize() {
	advance();
	while (!is_at_end()) {
		if (previous().type() == SEMICOLON) { return; }
		switch (peek().type()) {
			case CLASS: case FUN: case FOR: case IF: case PRINT: case RETURN: case VAR: case WHILE: return;
			default:
				break;
		}
		advance();
	}
}
