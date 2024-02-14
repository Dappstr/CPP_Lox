#pragma once

#include "token.hpp"

#include <memory>
#include <vector>
#include <variant>
#include <string>
#include <initializer_list>

using variant_type = std::optional<std::variant<std::string, double, bool>>;

// The `Expr_Visitor` class is a design pattern for viewing the representation of an expression without directly modifying it
class Expr_Visitor;

class Expr {
public:
  virtual void accept(Expr_Visitor* visitor) = 0;
  virtual ~Expr() = default;
};

class Ternary : public Expr {
private:
  std::unique_ptr<Expr> m_cond;
  std::unique_ptr<Expr> m_then; // Expression to return on the left-hand side of the `:`
  std::unique_ptr<Expr> m_else; // Expression to return on the right-hand side of the `:`

public:
  Ternary(std::unique_ptr<Expr> expr,
		  std::unique_ptr<Expr> left,
		  std::unique_ptr<Expr> right);
  Expr* get_cond() const;
  Expr* get_then() const; // Return the expression on the left-hand side of the `:`
  Expr* get_else() const; // Return the expression on the right-hand side of the `:`
  void accept(Expr_Visitor* visitor) override;
};

class Binary : public Expr {
private:
  std::unique_ptr<Expr> m_left;
  Token m_op;
  std::unique_ptr<Expr> m_right;

public:
  Binary(std::unique_ptr<Expr> left, const Token& op, std::unique_ptr<Expr> right);
  Expr* get_left() const;
  const Token& get_op() const;
  Expr* get_right() const;
  void accept(Expr_Visitor* visitor) override;
};

class Unary : public Expr {
private:
  Token m_op;
  std::unique_ptr<Expr> m_right;

public:
  Unary(const Token& op, std::unique_ptr<Expr> right);
  const Token& get_op() const;
  Expr* get_right() const;
  void accept(Expr_Visitor* visitor) override;
};

class Grouping : public Expr {
private:
  std::unique_ptr<Expr> m_expression;

public:
  Grouping(std::unique_ptr<Expr> expression);
  Expr* get_expr() const;
  void accept(Expr_Visitor* visitor) override;
};

class Literal : public Expr {
private:
  variant_type m_value;

public:
  Literal(variant_type value);
  variant_type get_literal() const;
  void accept(Expr_Visitor* visitor) override;
};

class Expr_Visitor {
public:
  virtual ~Expr_Visitor() = default;
  virtual void visit_ternary_expr(Ternary* expr) = 0;
  virtual void visit_binary_expr(Binary* expr) = 0;
  virtual void visit_unary_expr(Unary* expr) = 0;
  virtual void visit_grouping_expr(Grouping* expr) = 0;
  virtual void visit_literal_expr(Literal* expr) = 0;
};

class Ast_Printer : public Expr_Visitor {
private:
  std::string m_output;
  void parenthesize(const std::string& name, std::initializer_list<Expr*> exprs);
  void parenthesize(std::initializer_list<Expr*> exprs);

public:
  std::string print(Expr* expr);
  void visit_ternary_expr(Ternary* expr) override;
  void visit_binary_expr(Binary* expr) override;
  void visit_unary_expr(Unary* expr) override;
  void visit_grouping_expr(Grouping* expr) override;
  void visit_literal_expr(Literal* expr) override;
};
