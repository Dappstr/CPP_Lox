
#ifndef EXPR_HPP
#define EXPR_HPP

#pragma once

class Binary_Expr;
class Grouping_Expr;
class Literal_Expr;
class Unary_Expr;

class Expr_Visitor {
    public:
        virtual ~Expr_Visitor() = default;
        virtual void visitBinaryExpr(const Binary_Expr& expr) = 0;
        virtual void visitGroupingExpr(const Grouping_Expr& expr) = 0;
        virtual void visitLiteralExpr(const Literal_Expr& expr) = 0;
        virtual void visitUnaryExpr(const Unary_Expr& expr) = 0;
};

class Expr {
    public:
        virtual ~Expr() = default;
        virtual void accept(Expr_Visitor& visitor) const = 0;
};

#endif //EXPR_HPP
