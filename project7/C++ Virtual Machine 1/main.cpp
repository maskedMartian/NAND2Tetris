#include <iostream>
#include <string>

#include "CodeWriter.h"
#include "Parser.h"

/*
FROM THE BOOK:
The main program should construct a Parser to parse the VM input file and a
CodeWriter to generate code into the corresponding output file. It should then
march through the VM commands in the input file and generate assembly code for
each one of them.
*/

int main()
{
    Parser parser("test1.vm");
    CodeWriter codeWriter("test1.vm");

    while (parser.theFileHasMoreCommands()) {
        parser.advance();
        if (parser.commandType() == C_PUSH) {
            codeWriter.WritePushPop(parser.commandType(), parser.arg1(), parser.arg2());
        } else {
            codeWriter.writeArithmetic(parser.arg1());
        }
    }

    return 0;
}
