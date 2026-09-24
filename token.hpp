#pragma once

#include <string>

namespace moon{

    enum class TokenKind{
        Identifer,
        String,

        LeftParen,
        RightParen,
        LeftBrace,
        RightBrace,

        End
    };

    struct Token {
        TokenKind kind;
        std::string text;
        int line;
    };
}
