#pragma once

#include "token.hpp"
#include "expression.hpp"
#include <memory>
#include <vector>
#include <string>
#include <optional>

#ifndef STATEMENT_HPP
#define STATEMENT_HPP

class Statement_Visitor;

class Statement {
	public:
		virtual ~Statement() = default;
		virtual void accept(Statement_Visitor& visitor) const = 0;
};

class Expression_Statement final : public Statement {
	private:
		std::unique_ptr<Expression> m_expression;
	public:
		Expression_Statement(std::unique_ptr<Expression>&& expression);
		void accept(Statement_Visitor& visitor) const override;
		const Expression& expression() const;
};

class Print_Statement final : public Statement {
	private:
		std::unique_ptr<Expression> m_expression;
	public:
		Print_Statement(std::unique_ptr<Expression>&& expression);
		void accept(Statement_Visitor& visitor) const override;
		const Expression& expression() const;
};

class Statement_Visitor {
	public:
		virtual void visit(const Expression_Statement& stmt) = 0;
		virtual  void visit(const Print_Statement& stmt) = 0;
		virtual ~Statement_Visitor() = default;
};

#endif //STATEMENT_HPP
