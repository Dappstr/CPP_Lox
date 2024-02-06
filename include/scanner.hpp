#pragma once

#include "token.hpp"
#include "token_type.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <optional>
#include <variant>

class Scanner {
private:
  
  std::string m_source;
  std::vector<Token> m_tokens;
  size_t m_start = 0;
  size_t m_current = 0;
  size_t m_line = 1;

  std::unordered_map<std::string, Token_Type> m_keywords {
	{"and", Token_Type::AND},
	{"class", Token_Type::CLASS},
	{"else", Token_Type::ELSE},
	{"false", Token_Type::FALSE},
	{"for", Token_Type::FOR},
	{"fun", Token_Type::FUN},
	{"if", Token_Type::IF},
	{"nil", Token_Type::NIL},
	{"or", Token_Type::OR},
	{"print", Token_Type::PRINT},
	{"return", Token_Type::RETURN},
	{"super", Token_Type::SUPER},
	{"this", Token_Type::THIS},
	{"true", Token_Type::TRUE},
	{"var", Token_Type::VAR},
	{"while", Token_Type::WHILE}		
  };
  
public:
  Scanner(const std::string&);
  bool is_at_end();
  char peek();
  char peek_next();
  char advance();
  void add_token(Token_Type);
  void add_token(Token_Type, const std::optional<std::variant<std::string, double>>&);
  bool match(char);
  void string();
  void number();
  void identifier();
  void scan_token();
  const std::vector<Token>& scan_tokens();
};
