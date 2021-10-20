#include <iostream>
#include <string>

#include "Code.h"
#include "Output.h"
#include "Parser.h"
#include "SymbolTable.h"

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "ERROR: No source file specified";
        exit(1);
    }

    const std::string C_COMMAND_PREFIX = "111";
    const std::string SOURCE_FILE_NAME = argv[1];

    Parser parser{ SOURCE_FILE_NAME };
    Output output{ SOURCE_FILE_NAME };
    SymbolTable symbolTable{};
    Code code{};

    // first pass
    while (parser.hasMoreCommands()) {
        // read next command from source file
        parser.advance();
        // label
        if (parser.commandType() == "L_COMMAND") {
            if (!symbolTable.contains(parser.symbol())) {
                symbolTable.addEntry(parser.symbol(), parser.getAddress());
            }
        }
    }

    // set file pointer to first line of source file
    parser.resetFile();

    // second pass
    while (parser.hasMoreCommands()) {
        // read next command from source file
        parser.advance();
        // addressing command
        if (parser.commandType() == "A_COMMAND") {
            if (parser.isConstant()) {
                output.writeToBin(parser.getConstant());
            } else {
                if (!symbolTable.contains(parser.symbol())) {
                    symbolTable.addEntry(parser.symbol(), "new");
                }
                output.writeToBin(symbolTable.getAddress(parser.symbol()));
            }
        // computation command
        } else if (parser.commandType() == "C_COMMAND") { 
            output.writeToBin(C_COMMAND_PREFIX + code.comp(parser.comp()) + code.dest(parser.dest()) + code.jump(parser.jump()));
        // label
        } else if (parser.commandType() != "L_COMMAND") {
                std::cout << "ERROR: Unexpected command";
            exit(1);
        }
    }

    parser.closeFile();
    output.closeFile();

    return 0;
}
