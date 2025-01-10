#include "statement.hpp"

Expression_Statement::Expression_Statement(std::unique_ptr<Expression>&& expression) : m_expression(std::move(expression)) {}
const Expression &Expression_Statement::expression() const { return *m_expression; }

Print_Statement::Print_Statement(std::unique_ptr<Expression> &&expression) :m_expression(std::move(expression)) {}
const Expression& Print_Statement::expression() const { return *m_expression; }

void Expression_Statement::accept(Statement_Visitor &visitor) const { visitor.visit(*this); }

void Print_Statement::accept(Statement_Visitor &visitor) const { visitor.visit(*this); }