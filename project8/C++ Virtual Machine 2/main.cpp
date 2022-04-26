
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

void translateFile(std::string, CodeWriter*);

int main(int argc, char* argv[])
{
    std::string root = "./";

    if (COMMAND_LINE_PARAMETER_WAS_ENTERED) {
        std::string parameter{ argv[FIRST_PARAMETER] };
        int startingPosition = parameter.length() - EXTENSION_LENGTH;
        std::string extension = parameter.substr(startingPosition, std::string::npos);
        if (extension == ".vm") {
            CodeWriter* codeWriter = new CodeWriter(parameter);
            translateFile(parameter, codeWriter);
        } else {
            // parameter is a directory
            std::string directory = parameter + '\\' + parameter;
            CodeWriter* codeWriter = new CodeWriter(directory);
            codeWriter->writeInit();
            std::string path = root + parameter;
            for (const auto& file : std::filesystem::recursive_directory_iterator(path)) {
                std::string filepath = file.path().string();
                startingPosition = filepath.length() - EXTENSION_LENGTH;
                extension = filepath.substr(startingPosition, std::string::npos);
                if (extension == ".vm") {
                    startingPosition = root.length();
                    std::string filename = filepath.substr(startingPosition, std::string::npos);
                    translateFile(filename, codeWriter);
                }
            }
        } 
    } else {
        std::cout << "ERROR: no filename was specified";
    }
    return 0;
}

void translateFile(std::string filename, CodeWriter* codeWriter)
{
    Parser parser(filename);
    codeWriter->setFileName(filename);

    while (parser.theFileHasMoreCommands()) {
        parser.advance();
        if (parser.commandType() == C_PUSH || parser.commandType() == C_POP) {
            codeWriter->writePushPop(parser.commandType(), parser.arg1(), parser.arg2());
        } else if (parser.commandType() == C_ARITHMETIC) {
            codeWriter->writeArithmetic(parser.arg1());
        } else if (parser.commandType() == C_LABEL) {
            codeWriter->writeLabel(parser.arg1());
        } else if (parser.commandType() == C_GOTO) {
            codeWriter->writeGoto(parser.arg1());
        } else if (parser.commandType() == C_IF) {
            codeWriter->writeIf(parser.arg1());
        } else if (parser.commandType() == C_CALL) {
            codeWriter->writeCall(parser.arg1(), parser.arg2());
        } else if (parser.commandType() == C_RETURN) {
            codeWriter->writeReturn();
        } else { // parser.commandType() == C_FUNCTION
            codeWriter->writeFunction(parser.arg1(), parser.arg2());
        }
    }
}
