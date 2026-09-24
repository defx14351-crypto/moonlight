#pragma once

#include <memory>
#include <string>
#include <vector>

namespace moon {

struct Statement {
    virtual ~Statement() = default;
};

struct PrintStatement final : Statement {
    explicit PrintStatement(std::string value)
        : value(std::move(value)) {
    }

    std::string value;
};

struct Program {
    std::vector<std::unique_ptr<Statement>> statements;
};

}
