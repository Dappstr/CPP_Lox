#include "../include/expr.hpp"
#include "../include/token.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <initializer_list>
#include <optional>
#include <variant>

Ternary::Ternary(std::unique_ptr<Expr> expr, 
				 std::unique_ptr<Expr> left,
				 std::unique_ptr<Expr> right) {
  this->m_cond = std::move(expr);
  this->m_then = std::move(left);
  this->m_else = std::move(right);
}

Expr* Ternary::get_cond() const { return this->m_cond.get(); }
Expr* Ternary::get_then() const { return this->m_then.get(); }
Expr* Ternary::get_else() const { return this->m_else.get(); }
void Ternary::accept(Expr_Visitor* visitor) { visitor->visit_ternary_expr(this); }

Binary::Binary(std::unique_ptr<Expr> left, const Token& op, std::unique_ptr<Expr> right) {
  this->m_left = std::move(left);
  this->m_op = op;
  this->m_right = std::move(right);
}

// We return a raw pointer because we're not wanting to transfer ownership of the data it points to.
Expr* Binary::get_left() const { return this->m_left.get(); }
const Token& Binary::get_op() const { return this->m_op; }
Expr* Binary::get_right() const { return this->m_right.get(); }
void Binary::accept(Expr_Visitor* visitor) { visitor->visit_binary_expr(this); }


Unary::Unary(const Token& op, std::unique_ptr<Expr> right) {
  this->m_op = op;
  this->m_right = std::move(right);
}
const Token& Unary::get_op() const { return this->m_op; }
Expr* Unary::get_right() const { return this->m_right.get(); }
void Unary::accept(Expr_Visitor* visitor) { visitor->visit_unary_expr(this); }


Grouping::Grouping(std::unique_ptr<Expr> expression) { this->m_expression = std::move(expression); }
Expr* Grouping::get_expr() const { return this->m_expression.get(); }
void Grouping::accept(Expr_Visitor* visitor) { visitor->visit_grouping_expr(this); }


Literal::Literal(variant_type value) { this->m_value = value; }
variant_type Literal::get_literal() const {
  if(this->m_value.has_value()) {
	return this->m_value;
  }
  else {
	return std::nullopt;
  }
}
void Literal::accept(Expr_Visitor* visitor) { visitor->visit_literal_expr(this); }

// Will separate different expression types into parenthesized groups
void Ast_Printer::parenthesize(const std::string& name, std::initializer_list<Expr*> exprs) {
  this->m_output += "(" + name;
  for(Expr* expr : exprs) {
	m_output += " ";
	if(expr) { expr->accept(this); }
	else { m_output += "nil"; }
  }
  m_output += ")";
}

void Ast_Printer::parenthesize(std::initializer_list<Expr*> exprs) {
  this->m_output += "(?:";
  for(Expr* expr : exprs) {
	m_output += " ";
	if(expr) { expr->accept(this); }
	else { m_output += "nil"; }
  }
  m_output += ")";
}

std::string Ast_Printer::print(Expr* expr) {
  if(expr == nullptr) { return "nil"; }
  else {
	expr->accept(this);
	return m_output;
  }
}


void Ast_Printer::visit_ternary_expr(Ternary* expr) {
  parenthesize({expr->get_cond(), expr->get_then(), expr->get_else()});
}

void Ast_Printer::visit_binary_expr(Binary* expr) {
  parenthesize(expr->get_op().get_lexeme(), {expr->get_left(), expr->get_right()});
}

void Ast_Printer::visit_unary_expr(Unary* expr) {
  parenthesize(expr->get_op().get_lexeme(), {expr->get_right()});
}

void Ast_Printer::visit_grouping_expr(Grouping* expr) {
  parenthesize("group", {expr->get_expr()});
}

void Ast_Printer::visit_literal_expr(Literal* expr) {
  if(!expr->get_literal().has_value()) {
	m_output += "nil";
  }
  else {
	m_output += "Literal: ";
	if(std::holds_alternative<std::string>(expr->get_literal().value())) {
	  std::string lit = std::get<std::string>(expr->get_literal().value());
	  m_output += lit;
	}
	else if(std::holds_alternative<double>(expr->get_literal().value())) {
	  double lit = std::get<double>(expr->get_literal().value());
	  m_output += std::to_string(lit);
	}
	else {
	  bool lit = std::get<bool>(expr->get_literal().value());
	  m_output += lit ? "true" : "false";
	}
  }
}
