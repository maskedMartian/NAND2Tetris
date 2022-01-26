
#include <filesystem>
#include <iostream>
#include <string>

#include "CodeWriter.h"
#include "Parser.h"

#define FIRST_COMMAND_LINE_PARAMETER 1

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
    std::cout << "--> 0";
    //if (argc > 1) {
        std::cout << "--> 1";
        //std::string parameter{ argv[FIRST_COMMAND_LINE_PARAMETER] };
        std::string parameter{ "test" };
        if (parameter.substr(parameter.length() - CHARS_IN_EXTENSION, CHARS_IN_EXTENSION) == ".vm") {
            std::cout << "--> 2";
            //translateFile(parameter);
        } else {
            std::cout << "--> 3";
            // parameter is a directory
            std::string path = "./" + parameter;
            std::cout << "--> 4";
            for (const auto& file : std::filesystem::recursive_directory_iterator(path)) {
                std::cout << "--> 5";
                if (file.path().string().substr(file.path().string().length() - CHARS_IN_EXTENSION, CHARS_IN_EXTENSION) == ".vm") {
                    //translateFile(file.path().string());
                    std::cout << "--> 6";
                    //std::string X = file.path().string();
                    //std::string pyt = file.path().string().substr(path.length() + 1, file.path().string().length());
                    std::string pyt = file.path().string().substr(2, file.path().string().length());
                    std::cout << "found one: " << pyt << "\n";
                    translateFile(pyt);
                }
            }
        } 
    //} else {
        //std::cout << "ERROR: no filename was specified";
    //}
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