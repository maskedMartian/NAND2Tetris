#include "CodeWriter.h"

// Opens the output file/stream and gets ready to write into it
CodeWriter::CodeWriter(std::string filename)
{
    currentFile = filename.substr(0, filename.find('.')) + ".asm";
    asmFile.open(currentFile);
}

// Closes the input file
CodeWriter::~CodeWriter()
{
    if (asmFile.is_open()) {
        asmFile.close();
    }
}

// Informs the code writer that the translation of a new VM file is started
void CodeWriter::setFileName(std::string filename)
{
    asmFile.close();
    currentFile = filename.substr(0, filename.find('.')) + ".asm";
    asmFile.open(currentFile);
}

// Writes the assembly code that is that is the translation of the given arithmetic command
void CodeWriter::writeArithmetic(std::string command)
{
    switch (operation[command]) {
    case add:
        popStackToRegisterD();
        popStackToRegisterA();
        asmFile << "D=D+M\n";
        pushRegisterDToStack();
        break;
    case sub:
        popStackToRegisterD();
        popStackToRegisterA();
        asmFile << "D=D-M\n";
        pushRegisterDToStack();
        break;
    default:
        break;
    }
}

// Writes the assembly code that is the translation of the given command, where command is C_PUSH
// or C_POP
void CodeWriter::WritePushPop(CommandTypes command, std::string segment, int index)
{
    if (command == C_PUSH) {
        if (segment == "constant") {
            asmFile << "@" << index << "\n"
                    << "D=A\n";
            pushRegisterDToStack();
        }
    // command == C_POP
    } else {
        if (segment == "constant") {

        }
    }
}

// Closes the output file
void CodeWriter::close()
{
    asmFile.close();
}

// Print assembly commands to assembly file that will pop a value from the stack into register A
void CodeWriter::popStackToRegisterA()
{
    asmFile << "@SP\n"
            << "M=M-1\n"
            << "A=M\n";
}

// Print assembly commands to assembly file that will pop a value from the stack into register D
void CodeWriter::popStackToRegisterD()
{
    popStackToRegisterA();
    asmFile << "D=M\n";
}

//
void CodeWriter::pushRegisterDToStack()
{
    asmFile << "@SP\n"
        << "A=M\n"
        << "M=D\n"
        << "@SP\n"
        << "M=M+1\n";
}