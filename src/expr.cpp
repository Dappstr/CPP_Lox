#include "../include/expr.hpp"
#include "../include/token.hpp"

#include <memory>

Binary::Binary(std::unique_ptr<Expr> left, const Token& op, std::unique_ptr<Expr> right) {
  this->m_left = std::move(left);
  this->m_op = op;
  this->m_right = std::move(right);
}

Unary::Unary(const Token& op, std::unique_ptr<Expr> right) {
  this->m_op = op;
  this->m_right = std::move(right);
}

Grouping::Grouping(std::unique_ptr<Expr> expression) {
  this->m_expression = std::move(expression);
}

Literal::Literal(variant_type value) {
  this->m_value = value;
}
