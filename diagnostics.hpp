#pragma once

#include <string>

namespace moon{
    [[noreturn]]
    void error (
        const std::string& phase,
        int line,
        const std::string& message
    );
}
