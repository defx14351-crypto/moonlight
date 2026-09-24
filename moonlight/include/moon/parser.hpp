#pragma once

#include "moon/ast.hpp"
#include "moon/token.hpp"

#include <memory>
#include <vector>

namespace moon {

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);

    Program parse();

private:
    std::vector<Token> tokens_;
    std::size_t current_ = 0;

    std::unique_ptr<Statement> parseStatement();

    bool check(TokenKind kind) const;
    bool checkIdentifier(const std::string& value) const;

    Token consume(
        TokenKind kind,
        const std::string& expected
    );

    void consumeIdentifier(const std::string& value);
};

}
