
#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP

#pragma once

#include "expr.hpp"
#include <string>

class Ast_Printer : public Expr_Visitor {
    private:
        std::string m_output;
        std::string parenthesize(const std::string& name, const std::vector<std::shared_ptr<Expr>>& exprs);
    public:
        std::string print(const std::shared_ptr<Expr>& expr);

        void visitBinaryExpr(const Binary_Expr& expr) override;
        void visitGroupingExpr(const Grouping_Expr& expr) override;
        void visitLiteralExpr(const Literal_Expr& expr) override;
        void visitUnaryExpr(const Unary_Expr& expr) override;
};

#endif //AST_PRINTER_HPP
