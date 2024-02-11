#pragma once

#include "token_type.hpp"
#include <optional>
#include <variant>
#include <string>

class Token {
private:
  Token_Type m_type;
  std::string m_lexeme;
  std::optional<std::variant<std::string, double, bool>> m_literal;
  size_t m_line;

public:
  Token();
  Token(const Token_Type,
		const std::string&,
		const std::optional<std::variant<std::string, double, bool>>,
		const size_t);

  Token(const Token& t);
  Token& operator=(const Token& t);
  
  const std::string to_string();
  const std::string& get_lexeme() const;
  const Token_Type get_type() const;
  const size_t get_line() const;
  std::optional<std::variant<std::string, double, bool>> get_literal();
};
