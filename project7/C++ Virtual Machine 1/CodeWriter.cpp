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
        addEndOfProgramCode();
        asmFile.close();
    }
}

// Informs the code writer that the translation of a new VM file is started
void CodeWriter::setFileName(std::string filename)
{
    addEndOfProgramCode();
    asmFile.close();
    currentFile = filename.substr(0, filename.find('.')) + ".asm";
    asmFile.open(currentFile);
}

// Writes the assembly code to the assembly file that is that is the translation of the given
// arithmetic command
void CodeWriter::writeArithmetic(std::string command)
{
    popStackToRegisterD();
    // unary operations
    if (command == "neg" || command == "not") {
        asmFile << "D="
                << ((command == "neg") ? "-" : "!")
                << "D\n";
    } else {
        popStackToRegisterM();
        // binary operations
        switch (operation[command]) {
        case add:
            asmFile << "D=D+M\n";
            break;
        case sub:
            asmFile << "D=M-D\n";
            break;
        case AND:
            asmFile << "D=D&M\n";
            break;
        case OR:
            asmFile << "D=D|M\n";
            break;
            // "eq", "lt", or "gt"
        default:
            compareRegistersMAndD(command);
            break;
        }
    }
    pushRegisterDToStack();
}

// Writes the assembly code to the assembly file that is the translation of the given command, where
// command is C_PUSH or C_POP
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

// Writes the assembly code to the assembly file that will pop a value from the stack into
// register M
void CodeWriter::popStackToRegisterM()
{
    asmFile << "@SP\n"
            << "M=M-1\n"
            << "A=M\n";
}

// Writes the assembly code to the assembly file that will pop a value from the stack into
// register D
void CodeWriter::popStackToRegisterD()
{
    popStackToRegisterM();
    asmFile << "D=M\n";
}

// Writes the assembly code to the assembly file that will push the value in register D to the stack
void CodeWriter::pushRegisterDToStack()
{
    asmFile << "@SP\n"
            << "A=M\n"
            << "M=D\n"
            << "@SP\n"
            << "M=M+1\n";
}

// Writes the assembly code to the assembly file that will compare the value in register M to the
// value in register D using the supplied command
void CodeWriter::compareRegistersMAndD(std::string command)
{
    command = 'j' + command;
    for (auto& ch : command) ch = toupper(ch);
    asmFile << "D=M-D\n"
            << "@TRUE" << labelCounter << "\n"
            << "D;" << command << "\n"
            << "D=0\n"
            << "@END" << labelCounter << "\n"
            << "0;JMP\n"
            << "(TRUE" << labelCounter << ")\n"
            << "D=-1\n"
            << "(END" << labelCounter << ")\n";
    labelCounter++;
}

// Writes the assembly code to the assembly file that will signal the end of the program with an
// infinite loop
void CodeWriter::addEndOfProgramCode()
{
    asmFile << "(END)\n"
            << "@END\n"
            << "0;JMP\n";
}
