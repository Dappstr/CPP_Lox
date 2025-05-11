#include "../include/statement.hpp"

Expression_Stmt::Expression_Stmt(std::shared_ptr<Expr> expression)
    : m_expression(std::move(expression)) {}

void Expression_Stmt::accept(Stmt_Visitor& visitor) {
    visitor.visitExpressionStmt(*this);
}

Print_Stmt::Print_Stmt(std::shared_ptr<Expr> expression)
    : m_expression(std::move(expression)) {}

void Print_Stmt::accept(Stmt_Visitor& visitor) {
    visitor.visitPrintStmt(*this);
}

Var_Stmt::Var_Stmt(Token name, std::shared_ptr<Expr> value)
    : m_name(std::move(name)), m_value(std::move(value)) {}

void Var_Stmt::accept(Stmt_Visitor& visitor) {
    visitor.visitVarStmt(*this);
}
