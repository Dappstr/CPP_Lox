#pragma once
#include "token.hpp"
#include <memory>
#include <variant>

#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

using Value = std::optional<std::variant<std::string, double, bool>>;

class Expression_Visitor;

class Expression {
	public:
		virtual Value accept(Expression_Visitor& visitor) const = 0;
		virtual ~Expression() = default;
};

class Binary_Expression final : public Expression {
	private:
		std::shared_ptr<Expression> m_left;
		Token_Type m_op;
		std::shared_ptr<Expression> m_right;

	public:
		Binary_Expression(std::unique_ptr<Expression>&& left, Token_Type op, std::unique_ptr<Expression>&& right)
			:m_left(std::move(left)), m_op(op), m_right(std::move(right)) {}

		Value accept(Expression_Visitor& visitor) const override;
		const Expression& left() const { return *m_left; }
		const Token_Type op() const { return m_op; }
		const Expression& right() const { return *m_right; }
};

class Unary_Expression final : public Expression {
	private:
		Token_Type m_op;
		std::unique_ptr<Expression> m_right;

	public:
		Unary_Expression(Token_Type op, std::unique_ptr<Expression>&& right)
			:m_op(op), m_right(std::move(right)) {}

		Value accept(Expression_Visitor& visitor) const override;
		const Token_Type op() const { return m_op; }
		const Expression& right() const { return *m_right; }
};

class Group_Expression final : public Expression {
	private:
		std::unique_ptr<Expression> m_expression;

	public:
		explicit Group_Expression(std::unique_ptr<Expression>&& expression)
			:m_expression(std::move(expression)) {}

		Value accept(Expression_Visitor& visitor) const override;
		const Expression& expression() const { return *m_expression; }
};

class Literal_Expression final : public Expression {
	private:
		using Literal = std::variant<std::string, double, bool>;
		std::optional<Literal> m_value;

	public:
		explicit Literal_Expression(std::optional<Literal> value)
			:m_value(std::move(value)) {}

		Value accept(Expression_Visitor& visitor) const override;
		const Literal& value() const { return *m_value; }
};

class Expression_Visitor {
	public:
		virtual Value visit(const Binary_Expression& expr) = 0;
		virtual Value visit(const Unary_Expression& expr) = 0;
		virtual Value visit(const Group_Expression& expr) = 0;
		virtual Value visit(const Literal_Expression& expr) = 0;
		virtual ~Expression_Visitor() = default;
};

#endif //EXPRESSION_HPP
