
#include "../include/callable.hpp"
#include "../include/interpreter.hpp"

OptionalLiteral LoxFunction::call(Interpreter &interpreter, const std::vector<OptionalLiteral> &arguments) {
    const auto env = std::make_shared<Environment>(interpreter.environment());
    for (size_t i = 0; i < m_declaration.m_arguments.size(); ++i) {
        env->define(m_declaration.m_arguments[i].lexeme(), arguments[i]);
    }
    interpreter.executeBlock(m_declaration.m_statements, env);
    return std::nullopt;
}
