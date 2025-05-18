
#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <unordered_map>
#include <string>
#include "token.hpp"

class Environment {
    private:
        std::unordered_map<std::string, OptionalLiteral> m_variables;
        std::shared_ptr<Environment> m_parent;

    public:
        Environment() = default;

        explicit Environment(std::shared_ptr<Environment> parent) : m_parent(std::move(parent)) {}

        void define(const std::string& name, const OptionalLiteral& value) { m_variables[name] = value; }

        [[nodiscard]] OptionalLiteral get(const Token& name) const {
            auto it = m_variables.find(name.lexeme());
            if (it != m_variables.end()) {
                return it->second;
            }
            if (m_parent) { return m_parent->get(name); }
            throw std::runtime_error("Undefined variable '" + name.lexeme() + "'.");
        }

        void assign(const Token& name, const OptionalLiteral& value) {
            if (auto it = m_variables.find(name.lexeme()); it != m_variables.end()) {
                m_variables[name.lexeme()] = value;
                return;
            }
            if (m_parent) {
                m_parent->assign(name, value);
                return;
            }
            throw std::runtime_error("Undefined variable '" + name.lexeme() + "'.");
        }
};

#endif //ENVIRONMENT_HPP
