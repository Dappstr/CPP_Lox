
#ifndef CALLABLE_HPP
#define CALLABLE_HPP

#pragma once

#include "statement.hpp"
#include "token.hpp"
#include "interpreter.hpp"

#include <vector>


class LoxCallable {
    public:
        virtual size_t arity() const = 0;
        virtual OptionalLiteral call(Interpreter &interpreter, const std::vector<OptionalLiteral>& arguments) = 0;
        virtual ~LoxCallable() = default;
};

class LoxFunction : public LoxCallable {
    public:
        Function_Stmt m_declaration;
        std::shared_ptr<Environment> m_closure;

        explicit LoxFunction(Function_Stmt declaration, std::shared_ptr<Environment> closure)
            : m_declaration(std::move(declaration)), m_closure(std::move(closure)) {}
        [[nodiscard]] size_t arity() const override { return m_declaration.m_arguments.size(); }
        OptionalLiteral call(Interpreter &interpreter, const std::vector<OptionalLiteral>& arguments) override;
};

#endif //CALLABLE_HPP
