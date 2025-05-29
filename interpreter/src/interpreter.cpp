#include <utility>

#include "../include/interpreter.hpp"
#include "../include/callable.hpp"
#include "../include/statement.hpp"

void Interpreter::interpret(const std::vector<std::shared_ptr<Stmt>>& statements) {
    for (const auto& stmt: statements) {
        stmt->accept(*this);
    }
}

void Interpreter::visitLiteralExpr(const Literal_Expr &expr) { m_result = expr.m_value; }

void Interpreter::visitUnaryExpr(const Unary_Expr &expr) {
    expr.m_operand->accept(*this);
    const OptionalLiteral right = m_result;

    switch (expr.m_op.type()) {
        case TokenType::MINUS:{
            if (!right || !std::holds_alternative<double>(*right)) {
                throw std::runtime_error("Invalid operand");
            }
            m_result = - std::get<double>(*right);
            break;
        }
        case TokenType::BANG: {
            m_result = isTruthy(right);
            break;
        }
        case TokenType::INCREMENT:
        case TokenType::DECREMENT: {
            const auto var_expr = std::dynamic_pointer_cast<Variable_Expr>(expr.m_operand);
            if (!var_expr) {
                throw std::runtime_error(R"(Invalid operand for either '++' or '--', must be a variable.)");
            }
            OptionalLiteral value = m_environment->get(var_expr->m_name);
            if (!value || !std::holds_alternative<double>(*value)) {
                throw std::runtime_error(R"(Invalid operand for either '++' or '--', must be a variable.)");
            }

            const double delta = (expr.m_op.type() == TokenType::INCREMENT) ? 1.0 : -1.0;
            double result = std::get<double>(*value) + delta;

            m_environment->assign(var_expr->m_name, result);
            m_result = result;
            break;
        }
        default:
            throw std::runtime_error("Invalid unary operator");
    }
}

void Interpreter::visitBinaryExpr(const Binary_Expr &expr) {
    expr.m_left->accept(*this);
    const OptionalLiteral left = m_result;
    expr.m_right->accept(*this);
    const OptionalLiteral right = m_result;

    const auto type = expr.m_op.type();

    if (!left || !right) { throw std::runtime_error("Operands must not be nil."); }

    if (type == TokenType::EQUAL_EQUAL || type == TokenType::BANG_EQUAL) {
        bool eq = isEqual(left, right);
        m_result = (type == TokenType::EQUAL_EQUAL) ? eq : !eq;
    }

    if (type == TokenType::PLUS) {
        if (std::holds_alternative<double>(*left) && std::holds_alternative<double>(*right)) {
            m_result = std::get<double>(*left) + std::get<double>(*right);
        } else if (std::holds_alternative<std::string>(*left) && std::holds_alternative<std::string>(*right)) {
            m_result = std::get<std::string>(*left) + std::get<std::string>(*right);
        } else {
            throw std::runtime_error("Operands must either be numbers or two strings.");
        }
    }

    if (type == TokenType::STAR) {
        if (std::holds_alternative<std::string>(*left) && std::holds_alternative<double>(*right)) {
            const double times = std::get<double>(*right);
            auto str = std::get<std::string>(*left);
            std::string result;
            for (size_t i = 0; i < times; i++) { result += str; }
            m_result = result;
        } else if (std::holds_alternative<double>(*left) && std::holds_alternative<std::string>(*right)) {
            const double times = std::get<double>(*left);
            std::string str = std::get<std::string>(*left);
            std::string result = str;
            for (size_t i = 0; i < times; i++) { result += str; }
            m_result = result;
        } else if (std::holds_alternative<double>(*left) && std::holds_alternative<double>(*right)) {
            m_result = std::get<double>(*left) * std::get<double>(*right);
        } else {
            throw std::runtime_error("Unsupported operands for \'*\'");
        }
    }

    if (type == TokenType::MINUS || type == TokenType::SLASH) {
        if (!std::holds_alternative<double>(*left) || !std::holds_alternative<double>(*right)) {
            throw std::runtime_error("Operands must not be numbers.");
        }
        if (type == TokenType::MINUS) {
            m_result = std::get<double>(*left) - std::get<double>(*right);
        } else {
            if (std::get<double>(*right) == 0.0) { throw std::runtime_error("Error! Division by zero."); }
            m_result = std::get<double>(*left) / std::get<double>(*right);
        }
    }
    if (type == TokenType::GREATER || type == TokenType::GREATER_EQUAL ||
        type == TokenType::LESS || type == TokenType::LESS_EQUAL) {
        if (std::holds_alternative<double>(*left) && std::holds_alternative<double>(*right)) {
            const double l = std::get<double>(*left);
            const double r = std::get<double>(*right);
            if (type == TokenType::GREATER)        m_result = l > r;
            else if (type == TokenType::GREATER_EQUAL) m_result = l >= r;
            else if (type == TokenType::LESS)          m_result = l < r;
            else if (type == TokenType::LESS_EQUAL)    m_result = l <= r;
        } else if (std::holds_alternative<std::string>(*left) && std::holds_alternative<std::string>(*right)) {
            const auto l = std::get<std::string>(*left);
            const auto r = std::get<std::string>(*right);
            if (type == TokenType::GREATER) { m_result = l > r; }
            else if (type == TokenType::GREATER_EQUAL) { m_result = l >= r; }
            else if (type == TokenType::LESS) { m_result = l < r; }
            else m_result = l <= r;
        } else {
            throw std::runtime_error("Operands for comparison must either be two numbers or two strings.");
        }
    }
}

void Interpreter::visitGroupingExpr(const Grouping_Expr &expr) {
    if (expr.m_exprs.empty()) {
        m_result = std::nullopt;
    } else {
        expr.m_exprs.front()->accept(*this);
    }
}

void Interpreter::visitVarStmt(const Var_Stmt &stmt) {
    OptionalLiteral value = std::nullopt;
    if (stmt.m_value) {
        stmt.m_value->accept(*this);
        value = m_result;
    }
    m_environment->define(stmt.m_name.lexeme(), value);
}

void Interpreter::visitPrintStmt(const Print_Stmt& stmt) {
    stmt.m_expression->accept(*this);

    if (const OptionalLiteral value = m_result; !value.has_value()) {
        std::cout << "nil\n";
    } else if (std::holds_alternative<double>(*value)) {
        std::cout << std::get<double>(*value) << '\n';
    } else if (std::holds_alternative<std::string>(*value)) {
        std::cout << std::get<std::string>(*value) << '\n';
    } else if (std::holds_alternative<bool>(*value)) {
        std::cout << std::boolalpha << std::get<bool>(*value) << '\n';
    } else {
        std::cout << "[ERROR] Unknown literal type.\n";
    }
}

void Interpreter::visitExpressionStmt(const Expression_Stmt &stmt) {
    if (!stmt.m_expression) {
        std::cerr << "[ERROR] Null expression in Expression_Stmt.\n";
        return;
    }
    stmt.m_expression->accept(*this);
    m_result = std::nullopt;
}

void Interpreter::visitVariableExpr(const Variable_Expr& expr) { m_result = m_environment->get(expr.m_name); }

bool Interpreter::isTruthy(const OptionalLiteral &val) {
    if (!val.has_value()) { return false; }
    if (std::holds_alternative<bool>(val.value())) { return std::get<bool>(val.value()); }
    return true;
}

bool Interpreter::isEqual(const OptionalLiteral &lhs, const OptionalLiteral &rhs) {
    if (!lhs.has_value() || !rhs.has_value()) { return false; }
    if (!lhs.has_value() && !rhs.has_value()) { return true; }
    return lhs.value() == rhs.value();
}

void Interpreter::visitBlockStmt(const Block_Stmt &stmt) {
    auto new_env = std::make_shared<Environment>(m_environment);
    executeBlock(stmt.m_statements, new_env);
}

void Interpreter::executeBlock(const std::vector<std::shared_ptr<Stmt> > &statements, std::shared_ptr<Environment> new_env) {
    auto previous = m_environment;
    try {
        m_environment = std::move(new_env);
        for (const auto &stmt : statements) {
            if (stmt) stmt->accept(*this);
        }
    } catch (...) {
        m_environment = previous;
        throw;
    }
    m_environment = previous;
}

void Interpreter::visitAssignExpr(const Assign_Expr &expr) {
    expr.m_value->accept(*this);
    const OptionalLiteral value = m_result;
    m_environment->assign(expr.m_name, value);
    m_result = value;
}

void Interpreter::visitIfStmt(const If_Stmt &stmt) {
    stmt.m_condition->accept(*this);
    if (isTruthy(m_result)) {
        stmt.m_then_branch->accept(*this);
    } else if (stmt.m_else_branch) {
        stmt.m_else_branch->accept(*this);
    }
}

void Interpreter::visitWhileStmt(const While_Stmt &stmt) {
    while (true) {
        stmt.m_condition->accept(*this);
        if (!isTruthy(m_result)) { break; }

        stmt.m_body->accept(*this);
    }
}

void Interpreter::visitForStmt(const For_Stmt &stmt) {
    auto loop_env = std::make_shared<Environment>(m_environment);
    auto previous = m_environment;
    m_environment = loop_env;

    if (stmt.m_initializer) {
        stmt.m_initializer->accept(*this);
        if (const auto var_stmt = std::dynamic_pointer_cast<Var_Stmt>(stmt.m_initializer)) {
            const std::string name = var_stmt->m_name.lexeme();
            OptionalLiteral value = m_environment->get(var_stmt->m_name);

            if (!value.has_value() || !std::holds_alternative<double>(value.value())) {
                throw std::runtime_error("[ERROR] Unknown literal type for the for-loop initializer.");
            }
        }
    }
    while (true) {
        if (stmt.m_condition) {
            stmt.m_condition->accept(*this);
            if (!isTruthy(m_result)) {
                break;
            }
        }
        stmt.m_body->accept(*this);
        if (stmt.m_step) { stmt.m_step->accept(*this); }
    }
    m_environment = previous;
}

void Interpreter::visitFunctionStmt(const Function_Stmt &stmt) {
    auto function = std::make_shared<LoxFunction>(stmt);
    m_environment->define(stmt.m_name.lexeme(), std::move(function));
}

void Interpreter::visitCallExpr(const Call_Expr &expr) {
    expr.m_callee->accept(*this);
    const OptionalLiteral callee = m_result;

    std::vector<OptionalLiteral> args;
    for (const auto &arg : expr.m_args) {
        arg->accept(*this);
        args.emplace_back(m_result);
    }

    if (!callee.has_value() || !std::holds_alternative<std::shared_ptr<LoxCallable>>(callee.value())) {
        throw std::runtime_error("Error, attempted to call a nil value.");
    }

    const auto function = std::get<std::shared_ptr<LoxCallable>>(callee.value());
    if (args.size() != function->arity()) {
        throw std::runtime_error("Error, wrong number of arguments.");
    }
    m_result = function->call(*this, args);
}