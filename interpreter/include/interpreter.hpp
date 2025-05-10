
#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "expr.hpp"
#include "token.hpp"

class Interpreter : public Expr_Visitor {
    public:
        OptionalLiteral interpret(const std::shared_ptr<Expr>& expr);
        void visitLiteralExpr(const Literal_Expr &expr) override;
        void visitUnaryExpr(const Unary_Expr &expr) override;
        void visitBinaryExpr(const Binary_Expr &expr) override;
        void visitGroupingExpr(const Grouping_Expr &expr) override;

    private:
        OptionalLiteral m_result{};
        static bool isTruthy(const OptionalLiteral& val);
        static bool isEqual(const OptionalLiteral& lhs, const OptionalLiteral& rhs);
};

#endif //INTERPRETER_HPP
