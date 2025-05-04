#include "../include/scanner.hpp"

#include <iostream>
#include <vector>
#include <string>

std::vector<Token> Scanner::scan() {
    while (!isAtEnd()) {
        m_start = m_current;
        scanToken();
    }
    m_tokens.emplace_back(Token{TokenType::EndOfFile, "", std::nullopt, m_line});
    return m_tokens;
}

bool Scanner::isAtEnd() const {
    return m_current >= m_src.length();
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
        case '(': addNullToken(TokenType::LEFT_PAREN); break;
        case ')': addNullToken(TokenType::RIGHT_PAREN); break;
        case '{': addNullToken(TokenType::LEFT_BRACE); break;
        case '}': addNullToken(TokenType::RIGHT_BRACE); break;
        case ',': addNullToken(TokenType::COMMA); break;
        case '.': addNullToken(TokenType::DOT); break;
        case '-': addNullToken(TokenType::MINUS); break;
        case '+': addNullToken(TokenType::PLUS); break;
        case '/': {
            if (match('/')) {
                while (peek() != '\n' && !isAtEnd()) {
                    advance();
                }
            } else if (match('*')) {
                while (!isAtEnd()) {
                    if (peek() == '*' && peekNext() == '/') {
                        advance(); // Consume the star
                        advance(); // Then consume the slash
                        break;
                    } else {
                        if (peek() == '\n') { m_line++; }
                        advance();
                    }
                }
                if (isAtEnd()) {
                    std::cerr << "Error! Unterminated multiline comment on line: " << m_line << '\n';
                }
            } else {
                addNullToken(TokenType::SLASH);
            }
            break;
        }
        case '*': addNullToken(TokenType::STAR); break;
        case ';': addNullToken(TokenType::SEMICOLON); break;
        case '!': addNullToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
        case '=': addNullToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
        case '<': addNullToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
        case '>': addNullToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;

        case ' ':
        case '\r':
        case '\t':
            break;

        case '\n':
            m_line++;
            break;

        case '\"':
            string();
            break;

        default: {
            if (isdigit(c)) {
                number();
            } else if (isalpha(c) || c == '_') {
                identifier();
            } else {
                std::cerr << "Error! Unknown token: " << c << '\n';
                exit(1);
            }
        }
    }
}

char Scanner::advance() { return m_src.at(m_current++); }

char Scanner::peek() const {
    if (isAtEnd()) { return '\0'; }
    return m_src.at(m_current);
}

char Scanner::peekNext() const {
    if (isAtEnd() || m_current + 1 >= m_src.length()) { return '\0'; }
    return m_src.at(m_current + 1);
}

bool Scanner::match(const char expected) {
    if (isAtEnd()) {
        return false;
    } else {
        if (m_src.at(m_current) != expected) {
            return false;
        }
    }
    m_current++;
    return true;
}

void Scanner::string() {
    while (!isAtEnd() && peek() != '\"') {
        if (peek() == '\n') { m_line++; }
        advance();
    }
    if (isAtEnd()) {
        std::cerr << "Error! Unterminated string\n";
        exit(1);
    }
    advance();

    const std::string str = m_src.substr(m_start + 1, m_current - m_start - 2);
    addToken(TokenType::STRING, str);
}

void Scanner::number() {
    while (!isAtEnd() && isdigit(peek())) {
        advance();
    }
    if (peek() == '.' && isdigit(peekNext())) {
        advance();
        while (isdigit(peek())) {
            advance();
        }
    }
    std::string str_num = m_src.substr(m_start, m_current - m_start);
    addToken(TokenType::NUMBER, stod(str_num));
}

void Scanner::identifier() {
    while (!isAtEnd() && (isalnum(peek()) || peek() == '_')) { advance(); }
    std::string str = m_src.substr(m_start, m_current - m_start);
    auto it = keywords.find(str);
    if (it == keywords.end()) {
        addToken(TokenType::IDENTIFIER, str);
    } else {
        addNullToken(it->second);
    }
}

void Scanner::addNullToken(const TokenType &type) { addToken(type, std::nullopt); }

void Scanner::addToken(const TokenType &type, const OptionalLiteral &literal) {
    std::string text = m_src.substr(m_start, m_current - m_start);
    m_tokens.emplace_back(type, std::move(text), literal, m_line);
}
