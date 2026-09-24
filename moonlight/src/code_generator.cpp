#include "moon/code_generator.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace fs = std::filesystem;

namespace moon {

void CodeGenerator::generate(
    const Program& program,
    const std::string& outputPath
) {
    fs::path path(outputPath);

    if (path.has_parent_path()) {
        fs::create_directories(path.parent_path());
    }

    std::ofstream output(outputPath);

    if (!output) {
        throw std::runtime_error(
            "cannot create generated file: " + outputPath
        );
    }

    output << "#include <stdio.h>\n\n";
    output << "int main(void) {\n";

    for (const auto& statement : program.statements) {
        const auto* print =
            dynamic_cast<const PrintStatement*>(
                statement.get()
            );

        if (print == nullptr) {
            throw std::runtime_error(
                "code generator: unknown AST node"
            );
        }

        output << "    printf(\"%s\\n\", \""
               << escapeString(print->value)
               << "\");\n";
    }

    output << "    return 0;\n";
    output << "}\n";
}

std::string CodeGenerator::escapeString(
    const std::string& value
) const {
    std::string result;

    for (char character : value) {
        if (character == '\\') {
            result += "\\\\";
        } else if (character == '"') {
            result += "\\\"";
        } else {
            result += character;
        }
    }

    return result;
}

}
