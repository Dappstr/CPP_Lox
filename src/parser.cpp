#include "../include/parser.hpp"
#include "../include/token.hpp"
#include "../include/expr.hpp"
#include "../include/token_type.hpp"

#include <vector>
#include <memory>
#include <initializer_list>
#include <variant>
#include <optional>

Parser::Parser(const std::vector<Token>& tokens) {
  this->m_tokens = tokens;
}

std::unique_ptr<Expr> Parser::primary() {
  if(match({Token_Type::FALSE})) { return std::make_unique<Literal>(false); } 
  else if(match({Token_Type::TRUE})) { return std::make_unique<Literal>(true); }
  else if(match({Token_Type::NIL})) { return std::make_unique<Literal>(std::nullopt); }
  
  if(match({Token_Type::NUMBER, Token_Type::STRING})) {
	return std::make_unique<Literal>(this->m_tokens.at(this->m_current).get_literal());
  }
  
  if(match({Token_Type::LEFT_PAREN})) {
	auto expr = expression();
	consume(Token_Type::RIGHT_PAREN, "Expect \')\' after expression.");
	return std::make_unique<Grouping>(std::move(expr));
  }
}

std::unique_ptr<Expr> Parser::unary() {
  if(match({Token_Type::BANG, Token_Type::MINUS})) {
	Token op = previous();
	auto right = unary();
	auto expr = std::make_unique<Unary>(op, std::move(right));
	return expr;
  }
  return primary();
}

std::unique_ptr<Expr> Parser::factor() {
  auto expr = unary();
  while(match({Token_Type::SLASH, Token_Type::STAR})) {
	Token op = previous();
	auto right = unary();
	expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
  }
  return expr;
}

std::unique_ptr<Expr> Parser::term() {
  auto expr = factor();
  while(match({Token_Type::MINUS, Token_Type::PLUS})) {
	Token op = previous();
	auto right = factor();
	expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
  }
  return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
  auto expr = term();
  while(match({Token_Type::GREATER, Token_Type::GREATER_EQUAL, Token_Type::LESS, Token_Type::LESS_EQUAL})) {
	Token op = previous();
	auto right = term();
	expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
  }
  return expr;
}

std::unique_ptr<Expr> Parser::equality() {
  auto expression = comparison();
  while(match({Token_Type::BANG_EQUAL, Token_Type::EQUAL_EQUAL})) {
	Token op = previous();
	auto right = comparison();
	expression = std::make_unique<Binary>(std::move(expression), op, std::move(right));
  }
  return expression;
}

std::unique_ptr<Expr> Parser::expression() { return equality(); }
const Token& Parser::peek() { return this->m_tokens.at(this->m_current); }
const Token& Parser::previous() { return this->m_tokens.at(this->m_current - 1); }

const Token& Parser::consume(Token_Type type, std::string&& msg) {
  if(check(type)) { return advance(); }

  //throw std::runtime_error(peek(), msg);
}

bool Parser::is_at_end() { return peek().get_type() == Token_Type::End_Of_File; }

const Token& Parser::advance() {
  if(!is_at_end()) { this->m_current++; }
  return previous();
}

bool Parser::check(Token_Type type) {
  if(is_at_end()) { return false; }
  return peek().get_type() == type;
}

bool Parser::match(std::initializer_list<Token_Type> types) {
  for(Token_Type type : types) {
	if(check(type)) {
	  advance();
	  return true;
	}
  }
  return false;
}
