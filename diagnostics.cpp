#include "moon/diagnostics.hpp"

#include <stdexcept>

namespace moon {

[[noreturn]]
void error(
    const std::string& phase,
    int line,
    const std::string& message
) {
    throw std::runtime_error(
        phase +
        " error at line " +
        std::to_string(line) +
        ": " +
        message
    );
}

}
