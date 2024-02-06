#pragma once
#include <string>

static bool had_error = false;

int main(int agrc, char* argv[]);
void run(const std::string&);
void run_file(const char*);
void run_prompt();
void report(size_t, std::string&&, std::string&&);
void error(size_t, std::string&&);
