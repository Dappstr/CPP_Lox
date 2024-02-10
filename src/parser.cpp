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

// This function handles parsing the simplest elements of the language
std::unique_ptr<Expr> Parser::primary() {
  if(match({Token_Type::FALSE})) { return std::make_unique<Literal>(false); } 
  else if(match({Token_Type::TRUE})) { return std::make_unique<Literal>(true); }
  else if(match({Token_Type::NIL})) { return std::make_unique<Literal>(std::nullopt); }
  
  if(match({Token_Type::NUMBER, Token_Type::STRING})) {
	return std::make_unique<Literal>(this->m_tokens.at(this->m_current).get_literal());
  }

  // If a '(' is found
  if(match({Token_Type::LEFT_PAREN})) {

	/*
	  the first call to `expression()` is made, returning equality()
	  equality() calls comparison(), which then calls term()
	  term() then calls factor() which then calls unary()
	  unary() will check if the current expression contains a unary operator
	  Essentially, we start parsing parsing at the highest precedence
	  which are the unary operators.
	*/
	auto expr = expression();

	// Once expr is finally parsed, we check if the current token is of type RIGHT_PAREN
	// If it is, we advance to it, otherwise throw an exception with the 2nd argument as the message
	consume(Token_Type::RIGHT_PAREN, "Expect \')\' after expression.");

	// Return the expression (`expr`) inside of the parenthesis
	return std::make_unique<Grouping>(std::move(expr));
  }
}

//Will check for the unary form of '!' and '-'(negative number)
std::unique_ptr<Expr> Parser::unary() {
  if(match({Token_Type::BANG, Token_Type::MINUS})) {
	Token op = previous();

	//This recursive call to unary will either return a bang, minus, or the literal returned by `primary()
	// The `expr` object is them constructed with the operator, and the value saved in `right`
	auto right = unary();
	auto expr = std::make_unique<Unary>(op, std::move(right));
	return expr;
  }
  return primary();
}

// Multiplication and division
std::unique_ptr<Expr> Parser::factor() {
  auto expr = unary();
  while(match({Token_Type::SLASH, Token_Type::STAR})) {
	Token op = previous();
	auto right = unary();
	expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
  }
  return expr;
}

// addition and subtraction
std::unique_ptr<Expr> Parser::term() {
  auto expr = factor();
  while(match({Token_Type::MINUS, Token_Type::PLUS})) {
	Token op = previous();
	auto right = factor();
	expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
  }
  return expr;
}

// Comparison operators (>, >=, <, <=)
std::unique_ptr<Expr> Parser::comparison() {
  auto expr = term();
  while(match({Token_Type::GREATER, Token_Type::GREATER_EQUAL, Token_Type::LESS, Token_Type::LESS_EQUAL})) {
	Token op = previous();
	auto right = term();
	expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
  }
  return expr;
}

// Equality operators (!=, ==)
std::unique_ptr<Expr> Parser::equality() {
  auto expr = comparison();
  while(match({Token_Type::BANG_EQUAL, Token_Type::EQUAL_EQUAL})) {
	Token op = previous();
	auto right = comparison();
	expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
  }
  return expr;
}

std::unique_ptr<Expr> Parser::expression() { return equality(); }
const Token& Parser::peek() { return this->m_tokens.at(this->m_current); }
const Token& Parser::previous() { return this->m_tokens.at(this->m_current - 1); }

const Token& Parser::consume(Token_Type type, std::string&& msg) {
  if(check(type)) { return advance(); }
  else {
	std::string err = "Token Type: " + std::to_string(static_cast<int>(type)) + ' ' + msg;
	throw std::runtime_error(err);
  }
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

// Will check if any of the types in the arguments match the current token type
// If it does, advance to that token and return true
bool Parser::match(std::initializer_list<Token_Type> types) {
  for(Token_Type type : types) {
	if(check(type)) {
	  advance();
	  return true;
	}
  }
  return false;
}
