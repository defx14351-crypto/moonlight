#pragma once

#include "moon/token.hpp"

#include <string>
#include <vector>

namespace moon {

    class Lexer {
        public:
            explicit Lexer(std::string source);

            std::vector<Token> tokensize() ;
        private:
            std::string source_;
            std::vector<Token> tokens_;

            std::size_t current_ = 0;
            int line_ = 1;

            void readIdentifier();
            void readString();
            void addToken(TokenKind kind, const std::strin& text);
    }:
}
