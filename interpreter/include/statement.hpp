#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "expr.hpp"
#include "token.hpp"
#include <memory>

class Expression_Stmt;
class Print_Stmt;
class Var_Stmt;
class Block_Stmt;
class If_Stmt;
class While_Stmt;
class For_Stmt;

class Stmt_Visitor {
    public:
        virtual void visitExpressionStmt(const Expression_Stmt& stmt) = 0;
        virtual void visitPrintStmt(const Print_Stmt& stmt) = 0;
        virtual void visitVarStmt(const Var_Stmt& stmt) = 0;
        virtual void visitBlockStmt(const Block_Stmt& stmt) = 0;
        virtual void visitIfStmt(const If_Stmt& stmt) = 0;
        virtual void visitWhileStmt(const While_Stmt& stmt) = 0;
        virtual void visitForStmt(const For_Stmt& stmt) = 0;
        virtual ~Stmt_Visitor() = default;
};

class Stmt {
    public:
        virtual void accept(Stmt_Visitor& visitor) = 0;
        virtual ~Stmt() = default;
};

class Expression_Stmt : public Stmt {
    public:
        std::shared_ptr<Expr> m_expression;
        explicit Expression_Stmt(std::shared_ptr<Expr> expression);
        void accept(Stmt_Visitor& visitor) override;
};

class Print_Stmt : public Stmt {
    public:
        std::shared_ptr<Expr> m_expression;
        explicit Print_Stmt(std::shared_ptr<Expr> expression);
        void accept(Stmt_Visitor& visitor) override;
};

class Var_Stmt : public Stmt {
    public:
        Token m_name;
        std::shared_ptr<Expr> m_value;
        Var_Stmt(Token name, std::shared_ptr<Expr> value);
        void accept(Stmt_Visitor& visitor) override;
};

class Block_Stmt : public Stmt {
    public:
        std::vector<std::shared_ptr<Stmt>> m_statements;
        explicit Block_Stmt(std::vector<std::shared_ptr<Stmt>> statements) : m_statements(std::move(statements)) {}
        void accept(Stmt_Visitor& visitor) override { visitor.visitBlockStmt(*this); }
};

class If_Stmt : public Stmt {
    public:
    std::shared_ptr<Expr> m_condition;
    std::shared_ptr<Stmt> m_then_branch;
    std::shared_ptr<Stmt> m_else_branch;

    If_Stmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> then_branch, std::shared_ptr<Stmt> else_branch)
        :m_condition(std::move(condition)), m_then_branch(std::move(then_branch)), m_else_branch(std::move(else_branch)) {}
    void accept(Stmt_Visitor& visitor) override { visitor.visitIfStmt(*this); }
};

class While_Stmt : public Stmt {
    public:
        std::shared_ptr<Expr> m_condition;
        std::shared_ptr<Stmt> m_body;

        While_Stmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
            :m_condition(std::move(condition)), m_body(std::move(body)) {}
        void accept(Stmt_Visitor& visitor) override { visitor.visitWhileStmt(*this); }
};

class For_Stmt : public Stmt {
    public:
        std::shared_ptr<Stmt> m_initializer;
        std::shared_ptr<Expr> m_condition;
        std::shared_ptr<Expr> m_step;
        std::shared_ptr<Stmt> m_body;

        For_Stmt(std::shared_ptr<Stmt> initializer, std::shared_ptr<Expr> condition, std::shared_ptr<Expr> step, std::shared_ptr<Stmt> body)
            :m_initializer(std::move(initializer)), m_condition(std::move(condition)), m_step(std::move(step)), m_body(std::move(body)) {}

        void accept(Stmt_Visitor& visitor) override { visitor.visitForStmt(*this); }
};

#endif // STATEMENT_HPP
