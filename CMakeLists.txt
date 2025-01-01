cmake_minimum_required(VERSION 3.10)
project(CPP_Lox)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Specify the output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add the executable and source files
add_executable(CPP_Lox
    src/main.cpp
    src/scanner.cpp
        include/expression.hpp
        src/expression.cpp
        include/parser.hpp
        src/parser.cpp
)

