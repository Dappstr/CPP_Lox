#include "../include/main.hpp"
#include "../include/scanner.hpp"
#include "../include/token.hpp"

#include <string>
#include <vector>
#include <optional>
#include <variant>

Scanner::Scanner(const std::string& source) {
  this->m_source = source;
}

bool Scanner::is_at_end() {
  return this->m_current >= this->m_source.length();
}

char Scanner::peek() {
  if(is_at_end()) { return '\0'; }
  return this->m_source.at(this->m_current);
}

char Scanner::peek_next() {
  if(this->m_current + 1 >= this->m_source.length()) { return '\0'; }
  return this->m_source.at(this->m_current + 1);
}

char Scanner::advance() {
  return this->m_source.at(this->m_current++);
}

void Scanner::add_token(Token_Type type) {
  add_token(type, std::nullopt);
}

void Scanner::add_token(Token_Type type, const std::optional<std::variant<std::string, double, bool>>& literal) {
  std::string text = this->m_source.substr(this->m_start, this->m_current - this->m_start);
  this->m_tokens.push_back(Token{type, text, literal, this->m_line});
}

bool Scanner::match(char expected) {
  if(is_at_end()) { return false; }
  if(this->m_source.at(this->m_current) != expected) { return false; }

  this->m_current++;
  return true;
}

void Scanner::string() {
  while(peek() != '\"' && !is_at_end()) {
	if(peek() == '\n') {
	  this->m_line++;
	}
	advance();
  }
  if(is_at_end()) {
	error(this->m_line, "Unterminated string!");
	return;
  }
  advance();
  std::string value = this->m_source.substr(this->m_start + 1, this->m_current - this->m_start - 2);
  add_token(Token_Type::STRING, value);
}

void Scanner::number() {  
  while(isdigit(peek())) {
	advance();
  }
  if(peek() == '.' && isdigit(peek_next())) {
	advance();
	while(isdigit(peek())) {
	  advance();
	}
  }
  std::string number_string = this->m_source.substr(this->m_start, this->m_current - this->m_start);
  double value = atof(number_string.c_str());  
  add_token(Token_Type::NUMBER, value);
}

void Scanner::identifier() {
  while(isalnum(peek()) || peek() == '_') { advance(); }
  std::string text = this->m_source.substr(this->m_start, this->m_current - this->m_start);
  Token_Type type;
  auto it = this->m_keywords.find(text);
  if(it != this->m_keywords.end()) {
	type = it->second;
  }
  else {
	type = Token_Type::IDENTIFIER;
  }
  add_token(type);
}

void Scanner::scan_token() {
  char c = advance();
  switch(c) {
  case '(': add_token(Token_Type::LEFT_PAREN); break;
  case ')':	add_token(Token_Type::RIGHT_PAREN); break;
  case '{': add_token(Token_Type::LEFT_BRACE); break;
  case '}': add_token(Token_Type::RIGHT_BRACE); break;
  case ',': add_token(Token_Type::COMMA); break;
  case '.': add_token(Token_Type::DOT); break;
  case '-': add_token(Token_Type::MINUS); break;
  case '+': add_token(Token_Type::PLUS); break;
  case ';': add_token(Token_Type::SEMICOLON); break ;
  case '*': add_token(Token_Type::STAR); break;
  case ':': add_token(Token_Type::COLON); break;
  case '?': add_token(Token_Type::QUESTION_MARK); break;
  case '!': match('=') ? add_token(Token_Type::BANG_EQUAL) : add_token(Token_Type::BANG); break;
  case '=': match('=') ? add_token(Token_Type::EQUAL_EQUAL) : add_token(Token_Type::EQUAL); break;
  case '<': match('=') ? add_token(Token_Type::LESS_EQUAL) : add_token(Token_Type::LESS); break;
  case '>': match('=') ? add_token(Token_Type::GREATER_EQUAL) : add_token(Token_Type::GREATER); break;
  case '/':
	if(match('/')) {
	  while(peek() != '\n' && !is_at_end()) {
		advance();
	  }
	}
	else if(match('*')) {
	  while((peek() != '*' && peek_next() != '/') && !is_at_end()) {
		if(peek() == '\n') { this->m_line++; }
		advance();
	  }
	}
	else {
	  add_token(Token_Type::SLASH);
	}
	break;
	
  case ' ':
  case '\r':
  case '\t':
	break;

  case '\n':
	this->m_line++;
	break;

  case '\"': string(); break;
	
  default:
	if(isdigit(c)) {
	  number();
	}
	else if(isalpha(c) || c == '_') {
	  identifier();
	}
	else {
	  error(this->m_line, "Unexpected character.");
	  break;
	}
  }
}

const std::vector<Token>& Scanner::scan_tokens() {
  while(!is_at_end()) {
	this->m_start = this->m_current;
	scan_token();
  }
  this->m_tokens.push_back(Token{Token_Type::End_Of_File, "", std::nullopt, this->m_line});
  return this->m_tokens;
}
