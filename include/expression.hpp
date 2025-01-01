#pragma once
#include "token.hpp"
#include <memory>
#include <variant>
#include <vector>

#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

class Expression_Visitor;

class Expression {
	public:
		virtual void accept(Expression_Visitor& visitor) = 0;
		virtual ~Expression() = default;
};

class Binary_Expression final : public Expression {
	private:
		std::shared_ptr<Expression> m_left;
		std::string m_op;
		std::shared_ptr<Expression> m_right;

	public:
		Binary_Expression(std::unique_ptr<Expression>&& left, std::string&& op, std::unique_ptr<Expression>&& right)
			:m_left(std::move(left)), m_op(std::move(op)), m_right(std::move(right)) {}

		void accept(Expression_Visitor& visitor) override;
		const Expression& left() const { return *m_left; }
		const std::string& op() const { return m_op; }
		const Expression& right() const { return *m_right; }
};

class Unary_Expression final : public Expression {
	private:
		std::string m_op;
		std::unique_ptr<Expression> m_right;

	public:
		Unary_Expression(std::string&& op, std::unique_ptr<Expression>&& right)
			:m_op(std::move(op)), m_right(std::move(right)) {}

		void accept(Expression_Visitor& visitor) override;
		const std::string& op() const { return m_op; }
		const Expression& right() const { return *m_right; }
};

class Group_Expression final : public Expression {
	private:
		std::unique_ptr<Expression> m_expression;

	public:
		explicit Group_Expression(std::unique_ptr<Expression>&& expression)
			:m_expression(std::move(expression)) {}

		void accept(Expression_Visitor& visitor) override;
		const Expression& expression() const { return *m_expression; }
};

class Literal_Expression final : public Expression {
	private:
		using Literal = std::variant<double, std::string, bool>;
		std::optional<Literal> m_value;

	public:
		explicit Literal_Expression(Literal&& value)
			:m_value(std::move(value)) {}

		void accept(Expression_Visitor& visitor) override;
		const Literal& value() const { return *m_value; }
};

class Expression_Visitor {
	public:
		virtual void visit(const Binary_Expression& expr) = 0;
		virtual void visit(const Unary_Expression& expr) = 0;
		virtual void visit(const Group_Expression& expr) = 0;
		virtual void visit(const Literal_Expression& expr) = 0;
		virtual ~Expression_Visitor() = default;
};

#endif //EXPRESSION_HPP
