#include "CodeWriter.h"

#define POINTER_SEGMENT (index + 3)
#define TEMP_SEGMENT    (index + 5)
#define R13             13

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
    std::string registerAorM = "M";

    if (command == C_PUSH) {
        // push the value of segment[index] onto the stack
        switch (segments[segment]) {
        case argument:
            loadSegmentAddressIntoRegisterA("ARG", index);
            break;
        case local:
            loadSegmentAddressIntoRegisterA("LCL", index);
            break;
        case STATIC:
            loadAddressOfStaticVariableIntoRegisterA(index);
            break;
        case constant:
            loadConstantValueIntoRegisterA(index);
            registerAorM = "A";
            break;
        case THIS:
            loadSegmentAddressIntoRegisterA("THIS", index);
            break;
        case that:
            loadSegmentAddressIntoRegisterA("THAT", index);
            break;
        case pointer:
            loadRamAddressIntoRegisterA(POINTER_SEGMENT);
            break;
        case temp:
            loadRamAddressIntoRegisterA(TEMP_SEGMENT);
            break;
        }
        setRegisterDEqualToRegister(registerAorM);
        pushRegisterDToStack();
    // (command == C_POP)
    } else {
        // pop the stack and store the value in segment[index]
        switch (segments[segment]) {
        case argument:
            loadSegmentAddressIntoRegisterA("ARG", index);
            break;
        case local:
            loadSegmentAddressIntoRegisterA("LCL", index);
            break;
        case STATIC:
            loadAddressOfStaticVariableIntoRegisterA(index);
            break;
        case THIS:
            loadSegmentAddressIntoRegisterA("THIS", index);
            break;
        case that:
            loadSegmentAddressIntoRegisterA("THAT", index);
            break;
        case pointer:
            loadRamAddressIntoRegisterA(POINTER_SEGMENT);
            break;
        case temp:
            loadRamAddressIntoRegisterA(TEMP_SEGMENT);
            break;
        }
        copyAddressFromRegisterAToRam(R13);
        popStackToRegisterD();
        copyRegisterDToAddressStoredInRam(R13);
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

// Writes the assembly code to the assembly file that will load the indexed address in the specified
// segment of RAM
void CodeWriter::loadSegmentAddressIntoRegisterA(std::string segment, int index)
{
    asmFile << "@" << segment << "\n"
            << "D=M\n"
            << "@" << index << "\n"
            << "A=D+A\n";
}

// Writes the assembly code to the assembly file that will copy the segment address from register A
// to RAM[13]
void CodeWriter::copyAddressFromRegisterAToRam(int address)
{
    asmFile << "D=A\n"
            << "@R" << address << "\n"
            << "M=D\n";
}

// Writes the assembly code to the assembly file that will copy the contents of register D to the
// memory segment address stored in RAM[13]
void CodeWriter::copyRegisterDToAddressStoredInRam(int address)
{
    asmFile << "@R" << address << "\n"
            << "A=M\n"
            << "M=D\n";
}

// Writes the assembly code to the assembly file that will copy the contents of a register to
// another register
void CodeWriter::setRegisterDEqualToRegister(std::string registerAorM)
{
    asmFile << "D=" << registerAorM << "\n";
}

// Writes the assembly code to the assembly file that will load a constant value into register A
void CodeWriter::loadConstantValueIntoRegisterA(int index)
{
    asmFile << "@" << index << "\n";
}

// Writes the assembly code to the assembly file that will load R0-R15 into register A
void CodeWriter::loadRamAddressIntoRegisterA(int address)
{
    asmFile << "@R" << address << "\n";
}

// Writes the assembly code to the assembly file that will load the memory address of a static
// variable into register A
void CodeWriter::loadAddressOfStaticVariableIntoRegisterA(int index)
{
    std::string filename = currentFile.substr(0, currentFile.find('.') + 1);
    while (filename.find('\\') != -1) {
        filename = filename.substr(filename.find('\\') + 1, std::string::npos);
    }
    asmFile << "@" << filename << index << "\n";
}