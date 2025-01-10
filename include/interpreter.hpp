#pragma once

#include "expression.hpp"
#include "parser.hpp"
#include "token.hpp"
#include "statement.hpp"

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

class Interpreter : public Expression_Visitor, public Statement_Visitor {
	private:
		void execute(const Statement &statement);
	public:
		Interpreter() = default;
		void interpret(const std::unique_ptr<Expression>& expression);
		void interpret(const std::vector<std::unique_ptr<Statement>>& statements);
		void visit(const Expression_Statement& stmt) override;
		void visit(const Print_Statement& stmt) override;
		Value visit(const Binary_Expression& expr) override;
		Value visit(const Unary_Expression& expr) override;
		Value visit(const Literal_Expression& expr) override;
		Value visit(const Group_Expression& expr) override;
		Value evaluate(const Expression& expr);
		virtual ~Interpreter() = default;
};

#endif //INTERPRETER_HPP
