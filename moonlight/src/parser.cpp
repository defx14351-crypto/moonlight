#include "moon/parser.hpp"
#include "moon/diagnostics.hpp"

#include <utility>

namespace moon {

Parser::Parser(std::vector<Token> tokens)
    : tokens_(std::move(tokens)) {
}

Program Parser::parse() {
    Program program;

    consumeIdentifier("int");
    consumeIdentifier("main");

    consume(TokenKind::LeftParen, "'('");
    consume(TokenKind::RightParen, "')'");
    consume(TokenKind::LeftBrace, "'{'");

    while (!check(TokenKind::RightBrace)) {
        if (check(TokenKind::End)) {
            error(
                "parser",
                tokens_[current_].line,
                "expected '}'"
            );
        }

        program.statements.push_back(parseStatement());
    }

    consume(TokenKind::RightBrace, "'}'");

    if (!check(TokenKind::End)) {
        error(
            "parser",
            tokens_[current_].line,
            "unexpected text after main function"
        );
    }

    return program;
}

std::unique_ptr<Statement> Parser::parseStatement() {
    if (!checkIdentifier("printC")) {
        error(
            "parser",
            tokens_[current_].line,
            "expected 'printC'"
        );
    }

    ++current_;

    consume(TokenKind::LeftParen, "'('");

    Token text = consume(
        TokenKind::String,
        "string"
    );

    consume(TokenKind::RightParen, "')'");

    return std::make_unique<PrintStatement>(text.text);
}

bool Parser::check(TokenKind kind) const {
    return tokens_[current_].kind == kind;
}

bool Parser::checkIdentifier(
    const std::string& value
) const {
    return check(TokenKind::Identifier) &&
           tokens_[current_].text == value;
}

Token Parser::consume(
    TokenKind kind,
    const std::string& expected
) {
    if (!check(kind)) {
        error(
            "parser",
            tokens_[current_].line,
            "expected " + expected
        );
    }

    return tokens_[current_++];
}

void Parser::consumeIdentifier(
    const std::string& value
) {
    if (!checkIdentifier(value)) {
        error(
            "parser",
            tokens_[current_].line,
            "expected '" + value + "'"
        );
    }

    ++current_;
}

}
