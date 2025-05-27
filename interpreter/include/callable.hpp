
#ifndef CALLABLE_HPP
#define CALLABLE_HPP

#pragma once

#include "statement.hpp"
#include "environment.hpp"
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

        explicit LoxFunction(Function_Stmt declaration) : m_declaration(std::move(declaration)) {}
        [[nodiscard]] size_t arity() const override { return m_declaration.m_arguments.size(); }
        OptionalLiteral call(Interpreter &interpreter, const std::vector<OptionalLiteral>& arguments) override;
};

#endif //CALLABLE_HPP
