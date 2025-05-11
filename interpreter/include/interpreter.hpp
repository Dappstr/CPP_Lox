
#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "expr.hpp"
#include "statement.hpp"
#include "token.hpp"

class Interpreter : public Expr_Visitor, public Stmt_Visitor {
    public:
        void interpret(const std::vector<std::shared_ptr<Stmt>>& statements);
        void visitLiteralExpr(const Literal_Expr &expr) override;
        void visitUnaryExpr(const Unary_Expr &expr) override;
        void visitBinaryExpr(const Binary_Expr &expr) override;
        void visitGroupingExpr(const Grouping_Expr &expr) override;
        void visitVarStmt(const Var_Stmt &stmt) override;
        void visitPrintStmt(const Print_Stmt &stmt) override;
        void visitExpressionStmt(const Expression_Stmt &stmt) override;

    private:
        OptionalLiteral m_result{};
        static bool isTruthy(const OptionalLiteral& val);
        static bool isEqual(const OptionalLiteral& lhs, const OptionalLiteral& rhs);
};

#endif //INTERPRETER_HPP
