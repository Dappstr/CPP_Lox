#include "../include/scanner.hpp"
#include <string>
#include <vector>

Scanner::Scanner(const std::string& source)
    :m_source(source) {}

std::vector<Token> Scanner::scan_tokens() {
    while(!is_at_end()) {
        m_start = m_current;
        scan_token();
    }
    m_tokens.push_back({EndOfFile, "", std::nullopt, m_line});
    return m_tokens;
}

void Scanner::scan_token() {
    char c = advance();
    switch(c) {
        case '(':
            add_null_token(LEFT_PAREN); break;
        case ')':
            add_null_token(RIGHT_PAREN); break;
        case '{':
            add_null_token(LEFT_BRACE); break;
        case '}':
            add_null_token(RIGHT_BRACE); break;
        case ',':
            add_null_token(COMMA); break;
        case '.':
            add_null_token(DOT); break;
        case '-':
            add_null_token(MINUS); break;
        case '+':
            add_null_token(PLUS); break;
        case ';':
            add_null_token(SEMICOLON); break;
        case '*':
            add_null_token(STAR); break;
        case '=':
            add_null_token(match('=') ? EQUAL_EQUAL : EQUAL); break;
        case '!':
            add_null_token(match('=') ? BANG_EQUAL : BANG); break;
        case '<':
            add_null_token(match('=') ? LESS_EQUAL : LESS); break;
        case '>':
            add_null_token(match('=') ? GREATER_EQUAL : GREATER); break;
        case '/':
            if(match('/')) {
                while(peek() != '\n' && !is_at_end()) {
                    advance();
                }
            }
            else {
                add_null_token(SLASH);
            }
            break;

        case ' ':
        case '\t':
        case '\r':
            break;
        
        case '\n':
            m_line += 1;
            break;

        case '\"':
            str();
            break;

        default:
            if(isdigit(c)) { number(); }
            else if(isalpha(c)) { identifier(); }
            else {
                std::cerr << m_line << ": Unexpected character: " << c << '\n';
                return;
            }
    }
}

bool Scanner::is_at_end() {
    return m_current >= m_source.length();
}

char Scanner::advance() {
    return m_source.at(m_current++);
}

void Scanner::add_null_token(const Token_Type type) {
    add_token(type, std::nullopt);
}

void Scanner::add_token(const Token_Type type, const Lit& literal) {
    const std::string str = m_source.substr(m_start, m_current - m_start);
    m_tokens.push_back({type, str, literal, m_line});
}

bool Scanner::match(const char expected) {
    if(is_at_end()) { return false; }
    if(m_source.at(m_current) != expected) { return false; }

    m_current++;
    return true;
}

char Scanner::peek() {
    if(is_at_end()) { return '\0'; }
    else {
        return m_source.at(m_current);
    }
}

char Scanner::peek_next() {
    if(m_current + 1 >= m_source.length()) { return '\0'; }
    else { return m_source.at(m_current + 1); }
}

void Scanner::str() {
    while(peek() != '\"' && !is_at_end()) {
        if(peek() == '\n') { m_line += 1;}
        advance();
    }

    if(is_at_end()) {
        std::cerr << "Line: " << m_line << ": Unterminated string.\n";
        return;
    }

    advance();

    std::string str = m_source.substr(m_start + 1, (m_current - m_start));
    add_token(STRING, str);
}

void Scanner::number() {
    while(isdigit(peek())) { advance(); }
    if(peek() == '.' && isdigit(peek_next())) { advance(); }
    while(isdigit(peek())) { advance(); }
    std::string str_number { m_source.substr(m_start, (m_current - m_start)) };
    double num = std::stod(str_number);
    add_token(NUMBER, num);
}

void Scanner::identifier() {
    while(isalnum(peek())) { advance(); }
    std::string text { m_source.substr(m_start, m_current - m_start) };

    auto it = m_keywords.find(text);
    if(it != m_keywords.end()) {
        add_null_token(it->second);
    }
    else {
        add_null_token(IDENTIFIER);
    }
}