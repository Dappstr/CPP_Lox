#include <iostream>
#include <fstream>
#include "../include/token.hpp"
#include "../include/scanner.hpp"
#include "../include/parser.hpp"
#include "../include/ast_printer.hpp"

static bool hadError = false;

void run(std::string&& src);

void report(const size_t line, const char *where, const char *msg) {
    std::cerr << "[ line " << line << "] Error: "<< where << ": " << msg << '\n';
    hadError = true;
}

void error(const size_t line, const char* msg) {
    report(line, "", msg);
}

// Get in the data from the file and store it in a string
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

        run(std::move(contents));
    }
}

void runPrompt() {
    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "quit" || input == "exit") break;

        run(std::move(input));
    }
}

void run(std::string&& src) {
    Scanner scanner(std::move(src));
    std::vector<Token> tokens = scanner.scan();

    Parser parser(std::move(tokens));
    std::shared_ptr<Expr> expression;

    try {
        expression = parser.parse();
    }   catch (const std::exception& e) {
        std::cerr << "Parser error: " << e.what() << '\n';
    }

    Ast_Printer printer;
    const std::string result = printer.print(expression);
    std::cout << result << '\n';
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
