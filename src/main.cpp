#include "../include/main.hpp"
#include "../include/scanner.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main (int argc, char* argv[]) {
  if(argc > 2) {
	std::cout << "Usage: cpplox <path_to_script>\n";
	return 64;
  }
  else if(argc == 2) {
	run_file(argv[1]);
  }
  else {
	run_prompt();
  }
  return 0;
}

void run(const std::string& input) {
  Scanner scanner = Scanner(input);
  std::vector<Token> tokens = scanner.scan_tokens();

  for(auto token : tokens) {
	std::cout << token.to_string() << '\n';
  }
}

void run_file(const char* path) {
  std::fstream file(path, std::ios::in);
  file.seekg(0, std::ios::end);
  size_t size = file.tellg();
  file.seekg(0, std::ios::beg);

  if(size == 0) {
	std::cerr << "File contents are empty.";
	return;
  }
  else if(size == 1) {
	std::cerr << "File contains only EOF.";
	return;
  }
  
  std::string contents(size, '\0');
  if(!file.read(&contents[0], size)) {
	file.close();
	throw std::runtime_error("Failed to read contents of file into buffer!\n");
  }
  run(contents);
  if(had_error) { std::exit(65); }
}

void run_prompt() {
  std::string input;

  while(true) {
	std::cout << "> ";
	std::getline(std::cin, input);
	run(input);
	had_error = false;
  }
}

void error(size_t line, std::string&& message) {
  report(line, "", std::move(message));
}

void report(size_t line, std::string&& where, std::string&& message) {
  std::cerr << "[Line " << line << "] Error" << where << ": " << message;
  had_error = true;
}
