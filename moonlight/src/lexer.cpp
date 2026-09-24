#include "moon/lexer.hpp"
#include "moon/diagnostics.hpp"

#include <cctype>
#include <utility>

namespace moon {

Lexer::Lexer(std::string source)
    : source_(std::move(source)) {
}

std::vector<Token> Lexer::tokenize() {
    while (current_ < source_.size()) {
        char character = source_[current_];

        if (character == '\n') {
            ++line_;
            ++current_;
            continue;
        }

        if (std::isspace(
                static_cast<unsigned char>(character)
            )) {
            ++current_;
            continue;
        }

        if (std::isalpha(
                static_cast<unsigned char>(character)
            ) || character == '_') {
            readIdentifier();
            continue;
        }

        switch (character) {
            case '(':
                addToken(TokenKind::LeftParen, "(");
                ++current_;
                break;

            case ')':
                addToken(TokenKind::RightParen, ")");
                ++current_;
                break;

            case '{':
                addToken(TokenKind::LeftBrace, "{");
                ++current_;
                break;

            case '}':
                addToken(TokenKind::RightBrace, "}");
                ++current_;
                break;

            case '"':
                readString();
                break;

            case ';':
                // Точка с запятой пока необязательна.
                ++current_;
                break;

            default:
                error("lexer", line_, "unknown character");
        }
    }

    tokens_.push_back({
        TokenKind::End,
        "",
        line_
    });

    return tokens_;
}

void Lexer::readIdentifier() {
    std::size_t start = current_;

    while (current_ < source_.size()) {
        char character = source_[current_];

        if (!std::isalnum(
                static_cast<unsigned char>(character)
            ) && character != '_') {
            break;
        }

        ++current_;
    }

    addToken(
        TokenKind::Identifier,
        source_.substr(start, current_ - start)
    );
}

void Lexer::readString() {
    ++current_;

    std::string value;

    while (current_ < source_.size() &&
           source_[current_] != '"') {
        if (source_[current_] == '\n') {
            error(
                "lexer",
                line_,
                "string cannot contain a new line"
            );
        }

        value += source_[current_];
        ++current_;
    }

    if (current_ >= source_.size()) {
        error("lexer", line_, "unterminated string");
    }

    ++current_;

    addToken(TokenKind::String, value);
}

void Lexer::addToken(
    TokenKind kind,
    const std::string& text
) {
    tokens_.push_back({
        kind,
        text,
        line_
    });
}

}
