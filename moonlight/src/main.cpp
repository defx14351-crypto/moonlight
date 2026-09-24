#include "moon/code_generator.hpp"
#include "moon/lexer.hpp"
#include "moon/parser.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

std::string readFile(const std::string& path) {
    std::ifstream file(path);

    if (!file) {
        throw std::runtime_error(
            "cannot open source file: " + path
        );
    }

    return {
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    };
}

void printHelp() {
    std::cout << "Moonlight compiler\n\n";
    std::cout << "Commands:\n";
    std::cout << "  moon build <file.m>\n";
    std::cout << "  moon run <file.m>\n";
    std::cout << "  moon check <file.m>\n";
}

int compileSource(
    const std::string& sourcePath
) {
    std::string source = readFile(sourcePath);

    moon::Lexer lexer(source);
    auto tokens = lexer.tokenize();

    moon::Parser parser(std::move(tokens));
    moon::Program program = parser.parse();

    if (!fs::exists("build")) {
        fs::create_directories("build");
    }

    moon::CodeGenerator generator;

    generator.generate(
        program,
        "build/generated.c"
    );

    std::string compileCommand =
        "g++ build/generated.c -o build/moon_program";

    int result = std::system(compileCommand.c_str());

    if (result != 0) {
        throw std::runtime_error(
            "C compiler failed"
        );
    }

    return 0;
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            printHelp();
            return 1;
        }

        std::string command = argv[1];
        std::string sourcePath = argv[2];

        if (command == "check") {
            std::string source = readFile(sourcePath);

            moon::Lexer lexer(source);
            auto tokens = lexer.tokenize();

            moon::Parser parser(std::move(tokens));
            parser.parse();

            std::cout << "check successful\n";
            return 0;
        }

        if (command == "build") {
            compileSource(sourcePath);

            std::cout << "build successful\n";
            std::cout << "output: build/moon_program\n";

            return 0;
        }

        if (command == "run") {
            compileSource(sourcePath);

            return std::system(
                "build/moon_program"
            );
        }

        printHelp();
        return 1;
    }
    catch (const std::exception& exception) {
        std::cerr << "error: "
                  << exception.what()
                  << '\n';

        return 1;
    }
}
