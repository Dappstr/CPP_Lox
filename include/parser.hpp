#pragma once

#ifndef PARSER_HPP
#define PARSER_HPP

#include "token.hpp"
#include "expression.hpp"

#include <vector>

class Parser {
	private:
		std::vector<Token> m_tokens;
		size_t m_current = 0;

	public:
		explicit Parser(std::vector<Token>&& tokens)
			:m_tokens(std::move(tokens)) {}
		std::unique_ptr<Expression> parse();
		std::unique_ptr<Expression> expression();
		std::unique_ptr<Expression> equality();
		std::unique_ptr<Expression> comparison();
		std::unique_ptr<Expression> term();
		std::unique_ptr<Expression> factor();
		std::unique_ptr<Expression> unary();
		std::unique_ptr<Expression> primary();

		template<typename... Types, typename = std::enable_if_t<(std::is_same_v<Types, Token_Type> && ...)>>
		bool match(Types... t) {
			std::vector<Token_Type> token_types {static_cast<Token_Type>(t)...};
			for (const auto& token : token_types) {
				if (check(token)) {
					advance();
					return true;
				}
			}
			return false;
		}
		bool check(Token_Type);
		Token& advance();
		bool is_at_end();
		Token& peek();
		Token& previous();
		Token& consume(Token_Type, const char*);
		void synchronize();
};


#endif //PARSER_HPP
