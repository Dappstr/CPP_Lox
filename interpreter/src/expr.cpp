#include <memory>
#include <utility>
#include "../include/expr.hpp"
#include "../include/token.hpp"

class Binary_Expr final : public Expr {
    public:
        std::shared_ptr<Expr> m_left, m_right;
        Token m_op;

        Binary_Expr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
            : m_left(std::move(left)), m_op(std::move(op)), m_right(std::move(right)) {}

        void accept(Expr_Visitor &visitor) const override {
            visitor.visitBinaryExpr(*this);
        }
};

class Grouping_Expr final : public Expr {
    public:
        std::vector<std::shared_ptr<Expr>> m_exprs;
        explicit Grouping_Expr(std::vector<std::shared_ptr<Expr>> exprs)
            :m_exprs(std::move(exprs)) {}

        void accept(Expr_Visitor &visitor) const override {
            visitor.visitGroupingExpr(*this);
        }
};

class Literal_Expr final : public Expr {
    public:
        Literal m_value;
        explicit Literal_Expr(Literal value) : m_value(std::move(value)) {}

        void accept(Expr_Visitor &visitor) const override {
            visitor.visitLiteralExpr(*this);
        }
};

class Unary_Expr final : public Expr {
    public:
        std::shared_ptr<Expr> m_operand;
        Token m_op;

        Unary_Expr(std::shared_ptr<Expr> operand, Token op)
            :m_operand(std::move(operand)), m_op(op) {}
        void accept(Expr_Visitor &visitor) const override {
            visitor.visitUnaryExpr(*this);
        }
}