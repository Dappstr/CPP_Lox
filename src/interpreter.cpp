#include "expression.hpp"
#include "interpreter.hpp"

void Interpreter::interpret(const std::unique_ptr<Expression>& expression) {
	if (const Value val = evaluate(*expression); std::holds_alternative<std::string>(*val)) {
		std::cout << std::get<std::string>(*val) << '\n';
	} else if (std::holds_alternative<double>(*val)){
		std::cout << std::get<double>(*val) << '\n';
	} else {
		std::cout << std::boolalpha << std::get<bool>(*val) << '\n';
	}
}

Value Interpreter::visit(const Binary_Expression &expr) {
	const auto left = evaluate(expr.left());
	const auto right = evaluate(expr.right());
	if (std::holds_alternative<std::string>(*left) && std::holds_alternative<std::string>(*right)) {
		switch (expr.op()) {
			case PLUS: // Concatenate strings
				return std::get<std::string>(*left) + std::get<std::string>(*right);
			case EQUAL_EQUAL:
				return std::get<std::string>(*left) == std::get<std::string>(*right);
			default:
				throw std::runtime_error("Unsupported operation for string literals");
		}
	}
	// Handle numeric literals
	if (std::holds_alternative<double>(*left) && std::holds_alternative<double>(*right)) {
		switch (expr.op()) {
			case PLUS: return std::get<double>(*left) + std::get<double>(*right);
			case MINUS: return std::get<double>(*left) - std::get<double>(*right);
			case STAR: return std::get<double>(*left) * std::get<double>(*right);
			case SLASH:
				if (std::get<double>(*right) == 0) {
					throw std::runtime_error("Division by zero");
				}
			return std::get<double>(*left) / std::get<double>(*right);
			case GREATER:
				return std::get<double>(*left) > std::get<double>(*right);
			case GREATER_EQUAL:
				return std::get<double>(*left) >= std::get<double>(*right);
			case LESS:
				return std::get<double>(*left) < std::get<double>(*right);
			case LESS_EQUAL:
				return std::get<double>(*left) <= std::get<double>(*right);
			case BANG_EQUAL:
				return std::get<double>(*left) != std::get<double>(*right);
			case EQUAL_EQUAL:
				return std::get<double>(*left) == std::get<double>(*right);
			default:
				throw std::runtime_error("Unsupported binary operator");
		}
	}
	throw std::runtime_error("Invalid operand types for binary operation");
}

Value Interpreter::visit(const Unary_Expression &expr) {
	const auto right = evaluate(expr.right());
	switch (expr.op()) {
		case MINUS: {
			if (!right.has_value()) {
				throw std::runtime_error("Operand must be a number. Got nil.");
			}
			if (std::holds_alternative<double>(*right)) {
				double negative = - std::get<double>(*right);
				return negative;
			}
			throw std::runtime_error("Operand must be a number.");
		}
		case BANG: {
			if (!right.has_value()) {
				throw std::runtime_error("Operand must be a number. Got nil.");
			}
			if (std::holds_alternative<double>(*right)) {
				return ! std::get<bool>(*right);
			}
		}
		default:
			throw std::runtime_error("Unsupported binary operation.");
	}
}

Value Interpreter::visit(const Literal_Expression &expr) { return expr.value(); }

Value Interpreter::visit(const Group_Expression &expr) { return evaluate(expr.expression()); }

Value Interpreter::evaluate(const Expression &expr) {
	return expr.accept(*this);
}
