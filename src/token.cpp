#include "../include/token.hpp"
#include <optional>
#include <variant>
#include <string>

Token::Token(const Token_Type type,
		const std::string& lexeme,
		const std::optional<std::variant<std::string, double>> literal,
		const size_t line) {
	this->m_type = type;
	this->m_lexeme = lexeme;
	this->m_literal = literal;
	this->m_line = line;
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
