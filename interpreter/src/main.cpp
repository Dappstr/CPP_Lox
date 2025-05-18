#include <iostream>
#include <fstream>
#include "../include/token.hpp"
#include "../include/scanner.hpp"
#include "../include/parser.hpp"
#include "../include/interpreter.hpp"

static bool hadError = false;

void run(std::string&& src, Interpreter& interpreter);

void report(const size_t line, const char *where, const char *msg) {
    std::cerr << "[ line " << line << "] Error: "<< where << ": " << msg << '\n';
    hadError = true;
}

void error(const size_t line, const char* msg) {
    report(line, "", msg);
}

void runFile(const char* path) {
    if (std::fstream file(path, std::ios::in); !file.is_open()) {
        std::cerr << "Failed to open file " << path << '\n';
    } else {
        file.seekg(0, std::ios::end);
        const size_t size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::string contents(size, '\0');
        file.read(&contents[0], size);
        file.close();

        Interpreter interpreter;
        run(std::move(contents), interpreter);
    }
}


void runPrompt() {
    Interpreter interpreter;

    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "quit" || input == "exit") break;
        if (input.empty()) continue; // Skip blank lines

        run(std::move(input), interpreter);
    }
}

void run(std::string&& src, Interpreter& interpreter) {
    Scanner scanner(std::move(src));
    std::vector<Token> tokens = scanner.scan();

    Parser parser(std::move(tokens));
    std::vector<std::shared_ptr<Stmt>> statements;

    try {
        statements = parser.parse();
        if (statements.empty()) {
            return;
        }
    } catch (const std::exception& e) {
        std::cerr << "Parser error: " << e.what() << '\n';
        return;
    }

    try {
        interpreter.interpret(statements);
    } catch (const std::exception& e) {
        std::cerr << "Runtime error: " << e.what() << '\n';
    }
}


int main(int argc, char *argv[]) {
    if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " [script]\n";
        return 64;
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        runPrompt();
    }
    return 0;
}
