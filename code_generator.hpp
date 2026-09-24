#pragma once

#include "moon/ast.hpp"

#include <string>

namespace moon {

class CodeGenerator {
public:
    void generate(
        const Program& program,
        const std::string& outputPath
    );

private:
    std::string escapeString(
        const std::string& value
    ) const;
};

}
