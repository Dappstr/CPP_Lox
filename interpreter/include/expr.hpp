
#ifndef EXPR_HPP
#define EXPR_HPP

#pragma once

#include <memory>
#include <variant>
#include <string>
#include "token.hpp"

class Binary_Expr;
class Grouping_Expr;
class Literal_Expr;
class Unary_Expr;

class Expr_Visitor {
    public:
        virtual void visitBinaryExpr(const Binary_Expr& expr) = 0;
        virtual void visitGroupingExpr(const Grouping_Expr& expr) = 0;
        virtual void visitLiteralExpr(const Literal_Expr& expr) = 0;
        virtual void visitUnaryExpr(const Unary_Expr& expr) = 0;
        virtual ~Expr_Visitor() = default;

};

class Expr {
    public:
        virtual ~Expr() = default;
        virtual void accept(Expr_Visitor& visitor) const = 0;
};

class Binary_Expr final : public Expr {
public:
    std::shared_ptr<Expr> m_left, m_right;
    Token m_op;

    Binary_Expr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
        : m_left(std::move(left)), m_right(std::move(right)), m_op(std::move(op)) {}

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
    OptionalLiteral m_value;
    explicit Literal_Expr(OptionalLiteral value) : m_value(std::move(value)) {}

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
};

#endif //EXPR_HPP
