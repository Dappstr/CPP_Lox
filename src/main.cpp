#include "../include/token.hpp"
#include "../include/scanner.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

static bool had_error = false;

void report(const size_t line, const std::string& where, const std::string& msg) {
    std::cout << "[line: " << line << "] Error " << where << ": " << msg << '\n';
    had_error = true;
}

void error(const size_t line, const std::string& message) {
    report(line, "", message);
}

void run(const std::string& src) {
    Scanner scanner = Scanner(src);
    std::vector<Token> tokens = scanner.scan_tokens();

    for(const Token& t : tokens) {
        std::cout << t << '\n';
    }
}

void run_file(const char* file_loc) {
    std::fstream file {file_loc, std::ios::in};
    if(!file.is_open()) {
        std::cerr << "Error opening file: " << file_loc;
        had_error = true;
        return;
    }
    file.seekg(0, std::ios::end);
    const size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string buffer(file_size, '\0');

    file.read(&buffer[0], file_size);
    file.close();
}

void run_prompt() {
    while(true) {
        std::cout << "> ";
        std::string line {};
        std::getline(std::cin, line);

        if(line == "quit()") {
            break;
        }
        else {
            run(line);
        }
    }
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        run_prompt();
    }
    else if(argc == 2) {
        run_file(argv[1]);
    }
    else {
        std::cerr << "Usage: " << argv[0] << " [script]";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}