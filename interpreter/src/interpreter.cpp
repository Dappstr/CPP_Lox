#include "../include/interpreter.hpp"

#include "../include/statement.hpp"

void Interpreter::interpret(const std::vector<std::shared_ptr<Stmt>>& statements) {
    // expr->accept(*this);
    // return m_result;
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

    if (!left || !right) {
        throw std::runtime_error("Operands must not be nil.");
    }

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
            std::string str = std::get<std::string>(*left);
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
    throw std::runtime_error("Not implemented.");
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
