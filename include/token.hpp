#pragma once

#include "token_type.hpp"
#include <optional>
#include <variant>
#include <string>

class Token {
private:
  Token_Type m_type;
  std::string m_lexeme;
  std::optional<std::variant<std::string, double>> m_literal;
  size_t m_line;

public:
  Token(const Token_Type,
		const std::string&,
		const std::optional<std::variant<std::string, double>>,
		const size_t);

  const std::string to_string();
};
