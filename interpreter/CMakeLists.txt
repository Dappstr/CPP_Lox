cmake_minimum_required(VERSION 3.30)
project(interpreter)

set(CMAKE_CXX_STANDARD 20)

add_executable(interpreter src/main.cpp
        include/token.hpp
        include/scanner.hpp
        src/scanner.cpp
        include/expr.hpp
        include/parser.hpp
        src/parser.cpp
        include/ast_printer.hpp
        src/ast_printer.cpp
        src/interpreter.cpp
        include/interpreter.hpp
        src/statement.cpp
        include/statement.hpp
        include/environment.hpp
        src/callable.cpp
        include/callable.hpp
)
