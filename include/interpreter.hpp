#pragma once

#include "expression.hpp"
#include "parser.hpp"
#include "token.hpp"
#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

class Interpreter : public Expression_Visitor {
	public:
		Interpreter() = default;
		void interpret(const std::unique_ptr<Expression>& expression);
		Value visit(const Binary_Expression& expr) override;
		Value visit(const Unary_Expression& expr) override;
		Value visit(const Literal_Expression& expr) override;
		Value visit(const Group_Expression& expr) override;
		Value evaluate(const Expression& expr);
		virtual ~Interpreter() = default;
};

#endif //INTERPRETER_HPP
