
#include <filesystem>
#include <iostream>
#include <string>

#include "CodeWriter.h"
#include "Parser.h"

#define FIRST_PARAMETER 1
#define COMMAND_LINE_PARAMETER_WAS_ENTERED (argc > 1)

/*
FROM THE BOOK:
The main program should construct a Parser to parse the VM input file and a
CodeWriter to generate code into the corresponding output file. It should then
march through the VM commands in the input file and generate assembly code for
each one of them.
*/

void translateFile(std::string);

int main(int argc, char* argv[])
{
    std::string root = "./";

    if (COMMAND_LINE_PARAMETER_WAS_ENTERED) {
        std::string parameter{ argv[FIRST_PARAMETER] };
        int startingPosition = parameter.length() - EXTENSION_LENGTH;
        std::string extension = parameter.substr(startingPosition, std::string::npos);
        if (extension == ".vm") {
            translateFile(parameter);
        } else {
            // parameter is a directory
            std::string path = root + parameter;
            for (const auto& file : std::filesystem::recursive_directory_iterator(path)) {
                std::string filepath = file.path().string();
                startingPosition = filepath.length() - EXTENSION_LENGTH;
                extension = filepath.substr(startingPosition, std::string::npos);
                if (extension == ".vm") {
                    startingPosition = root.length();
                    std::string filename = filepath.substr(startingPosition, std::string::npos);
                    translateFile(filename);
                }
            }
        } 
    } else {
        std::cout << "ERROR: no filename was specified";
    }
    return 0;
}

void translateFile(std::string filename)
{
    Parser parser(filename);
    CodeWriter codeWriter(filename);

    while (parser.theFileHasMoreCommands()) {
        parser.advance();
        if (parser.commandType() == C_PUSH || parser.commandType() == C_POP) {
            codeWriter.WritePushPop(parser.commandType(), parser.arg1(), parser.arg2());
        } else {
            codeWriter.writeArithmetic(parser.arg1());
        }
    }
}