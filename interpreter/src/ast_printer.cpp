#include "../include/ast_printer.hpp"

std::string Ast_Printer::parenthesize(const std::string &name, const std::vector<std::shared_ptr<Expr>> &exprs) {
    std::string result = "(" + name;
    for (const auto &expr : exprs) {
        result += ' ';
        expr->accept(*this);
        result += m_output;
    }
    result += ')';
    return result;
}

std::string Ast_Printer::print(const std::shared_ptr<Expr> &expr) {
    m_output.clear();
    expr->accept(*this);
    return m_output;
}

void Ast_Printer::visitBinaryExpr(const Binary_Expr &expr) {
    m_output = parenthesize(expr.m_op.lexeme(), {expr.m_left, expr.m_right});
}

void Ast_Printer::visitGroupingExpr(const Grouping_Expr &expr) {
    m_output = parenthesize("group", expr.m_exprs);

}

void Ast_Printer::visitLiteralExpr(const Literal_Expr &expr) {
    if (!expr.m_value.has_value()) {
        m_output = "nil";
    } else if (std::holds_alternative<double>(*expr.m_value)) {
        m_output = std::to_string(std::get<double>(*expr.m_value));
    } else if (std::holds_alternative<std::string>(*expr.m_value)) {
        m_output = std::get<std::string>(*expr.m_value);
    } else if (std::holds_alternative<bool>(*expr.m_value)) {
        m_output = std::get<bool>(*expr.m_value) ? "true" : "false";
    }
}

void Ast_Printer::visitUnaryExpr(const Unary_Expr &expr) {
    m_output = parenthesize(expr.m_op.lexeme(), {expr.m_operand});
}
