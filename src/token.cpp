#include "../include/token.hpp"
#include <optional>
#include <variant>
#include <string>

Token::Token() {
  this->m_type = Token_Type::NIL;
  this->m_lexeme = "";
  this->m_literal = std::nullopt;
  this->m_line = 0;
}

Token::Token(const Token_Type type,
		const std::string& lexeme,
		const std::optional<std::variant<std::string, double, bool>> literal,
		const size_t line) {
	this->m_type = type;
	this->m_lexeme = lexeme;
	this->m_literal = literal;
	this->m_line = line;
}

Token::Token(const Token& t) {
  this->m_type = t.m_type;
  this->m_lexeme = t.m_lexeme;
  this->m_literal = t.m_literal;
  this->m_line = t.m_line;
}

Token& Token::operator =(const Token& t) {
  if(this != &t) {
	this->m_type = t.m_type;
	this->m_lexeme = t.m_lexeme;
	this->m_literal = t.m_literal;
	this->m_line = t.m_line;
  }
  return *this;
}

const std::string Token::to_string() {
  std::string token_string;
  int token_type_value = static_cast<int>(this->m_type);
  token_string += (std::to_string(token_type_value) + " ");
  if(this->m_literal.has_value()) {
	if(std::holds_alternative<std::string>(this->m_literal.value())) {
	  token_string += std::get<std::string>(this->m_literal.value()) + " ";
	}
	else if(std::holds_alternative<double>(this->m_literal.value())) {
	  token_string += std::to_string(std::get<double>(this->m_literal.value())) + " ";
	}
  }
  token_string += this->m_lexeme;
  return token_string;
}

const Token_Type Token::get_type() const {
  return this->m_type;
}

const size_t Token::get_line() {
  return this->m_line;
}

std::optional<std::variant<std::string, double, bool>> Token::get_literal() {
  if(!this->m_literal.has_value()) {
	return std::nullopt;
  }
  return this->m_literal;
}
