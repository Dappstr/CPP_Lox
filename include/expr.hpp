#pragma once

#include "token.hpp"
#include <memory>
#include <vector>
#include <variant>
#include <string>

using variant_type = std::optional<std::variant<std::string, double, bool>>;

class Expr {
public:
  virtual ~Expr() = default;
};

class Binary : public Expr {
private:
  std::unique_ptr<Expr> m_left;
  Token m_op;
  std::unique_ptr<Expr> m_right;

public:
  Binary(std::unique_ptr<Expr> left, const Token& op, std::unique_ptr<Expr> right);
};

class Unary : public Expr {
private:
  Token m_op;
  std::unique_ptr<Expr> m_right;

public:
  Unary(const Token& op, std::unique_ptr<Expr> right);
};

class Grouping : public Expr {
private:
  std::unique_ptr<Expr> m_expression;

public:
  Grouping(std::unique_ptr<Expr> expression);
};

class Literal : public Expr {
private:
  variant_type m_value;

public:
  Literal(variant_type value);
};
