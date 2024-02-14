#pragma once

#include "token.hpp"
#include "token_type.hpp"
#include "expr.hpp"

#include <vector>
#include <memory>
#include <initializer_list>

class Parser {
private:
  std::vector<Token> m_tokens;
  size_t m_current = 0;
  
public:
  Parser(const std::vector<Token>& tokens);
  std::unique_ptr<Expr> parse();
  std::unique_ptr<Expr> primary();
  std::unique_ptr<Expr> unary();
  std::unique_ptr<Expr> factor();
  std::unique_ptr<Expr> term();
  std::unique_ptr<Expr> comparison();
  std::unique_ptr<Expr> equality();
  std::unique_ptr<Expr> ternary();
  std::unique_ptr<Expr> expression(); // returns equality()
  const Token& peek();
  const Token& previous();
  const Token& consume(Token_Type type, const std::string& msg);
  std::string error(const Token& token, const std::string& msg);
  void report(const size_t line, const std::string& where, const std::string& msg);
  void synchronize(); // For gracefully handling errors while parsing, we synchronize the parser
                      // up to the boundary where the next statement starts, or until we reach a semicolon
  bool is_at_end();
  const Token& advance();
  bool check(Token_Type type);
  bool match(std::initializer_list<Token_Type> types);
};
