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

// Writes the assembly code to the output file that is that is the translation of the given
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

// Writes the assembly code to the output file that is the translation of the given command, where
// command is C_PUSH or C_POP
void CodeWriter::writePushPop(CommandTypes command, std::string segment, int index)
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

// Write assembly code to the output file that effects the VM initialization, also called
// bootstrap code. This code must be placed at the beginning of the output file.
void CodeWriter::writeInit()
{

}

// Writes assembly code to the assembly file that effects the label command.
void CodeWriter::writeLabel(std::string label)
{
    asmFile << "(" << label << ")\n";
}

// Writes assembly code to the assembly file that effects the goto command.
void CodeWriter::writeGoto(std::string label)
{
    asmFile << "@" << label << "\n"
            << "0;JMP" << "\n";
}

// Writes assembly code to the assembly file that effects the if-goto command.
void CodeWriter::writeIf(std::string label)
{
    popStackToRegisterD();
    asmFile << "@" << label << "\n"
            << "D;JNE" << "\n";
}

// Writes assembly code to the assembly file that effects the call command.
void CodeWriter::writeCall(std::string functionName, int numArgs)
{

}

// Writes assembly code to the assembly file that effects the return command.
void CodeWriter::writeReturn()
{
    writePushPop(C_PUSH, "argument", 0); // push value at ARG[0] to stack
    popStackToRegisterD();
    incrementValueInRegisterD(); // increment the value stored in register D
    copyRegisterDToRam("R14"); // move the incremented value to R14
    writePushPop(C_POP, "argument", 0); // pop stack to ARG[0]
    copyRamToRam("SP", "LCL"); // SP = LCL
    writePushPop(C_POP, "argument", 0); // pop stack to ARG[0]
    popStackToRam("THAT"); // pop stack THAT (R4)
    popStackToRam("THIS"); // pop stack THIS (R3)
    popStackToRam("ARG"); // pop stack to ARG (R2)
    popStackToRam("LCL"); // pop stack to LCL (R1)
    popStackToRam("R15"); // pop stack to R15 (return address)
    copyRamToRam("SP", "R14"); // set SP = R14
    jumpToAddressStoredInRam("R15"); // jump to address stored in R15 - copy address into registerA
}

// Writes assembly code to the assembly file that effects the function command.
void CodeWriter::writeFunction(std::string functionName, int numLocals)
{
    writeLabel(functionName);
    loopXTimes(numLocals, [this]() {
        writePushPop(C_PUSH, "constant", 0);
    });
}

// Writes the assembly code to the output file that will pop a value from the stack into
// register M
void CodeWriter::popStackToRegisterM()
{
    asmFile << "@SP\n"
            << "M=M-1\n"
            << "A=M\n";
}

// Writes the assembly code to the output file that will pop a value from the stack into
// register D
void CodeWriter::popStackToRegisterD()
{
    popStackToRegisterM();
    asmFile << "D=M\n";
}

// Writes...
void CodeWriter::popStackToRam(std::string address)
{
    popStackToRegisterD();
    copyRegisterDToRam(address);
}

// Writes the assembly code to the output file that will push the value in register D onto the
// stack
void CodeWriter::pushRegisterDToStack()
{
    asmFile << "@SP\n"
            << "A=M\n"
            << "M=D\n"
            << "@SP\n"
            << "M=M+1\n";
}

// Writes the assembly code to the output file that will push the value in register M onto the
// stack
void CodeWriter::pushRegisterMToStack()
{
    asmFile << "D=M\n";
    pushRegisterDToStack();
}

// Writes...
void CodeWriter::pushRamToStack(std::string address)
{
    copyRamToRegisterD(address);
    pushRegisterDToStack();
}

// Writes...
void CodeWriter::copyRegisterDToRam(std::string address)
{
    asmFile << "@" << address << "\n"
            << "M=D\n";
}

// Writes...
void CodeWriter::copyRamToRegisterD(std::string address)
{
    asmFile << "@" << address << "\n"
            << "D=M\n";
}

// Writes...
void CodeWriter::copyRamToRam(std::string toAddress, std::string fromAddress)
{
    asmFile << "@" << fromAddress << "\n"
            << "D=M\n"
            << "@" << toAddress << "\n"
            << "M=D\n";
}

// Writes the assembly code to the output file that will compare the value in register M to the
// value in register D using the supplied command
void CodeWriter::compareRegistersMAndD(std::string command)
{
    for (auto& ch : command) ch = toupper(ch);
    command = 'J' + command; // moved this line from above the previous line to here - delete this comment after tested
    asmFile << "D=M-D\n"
            << "@TRUE" << labelCounter << "\n"
            << "D;" << command << "\n"
            << "D=0\n"
            << "@END" << labelCounter << "\n"
            << "0;JMP\n"
            << "(TRUE" << labelCounter << ")\n"
            << "D=-1\n"
            << "(END" << labelCounter << ")\n";
    ++labelCounter;
}

// Writes the assembly code to the output file that will signal the end of the program with an
// infinite loop
void CodeWriter::addEndOfProgramCode()
{
    asmFile << "(END)\n"
            << "@END\n"
            << "0;JMP\n";
}

// Writes the assembly code to the output file that will load the indexed address in the specified
// segment of RAM
void CodeWriter::loadSegmentAddressIntoRegisterA(std::string segment, int index)
{
    asmFile << "@" << segment << "\n"
            << "D=M\n"
            << "@" << index << "\n"
            << "A=D+A\n";
}

// Writes the assembly code to the output file that will copy the segment address from register A
// to RAM[13]
void CodeWriter::copyAddressFromRegisterAToRam(int address)
{
    asmFile << "D=A\n"
            << "@R" << address << "\n"
            << "M=D\n";
}

// Writes the assembly code to the output file that will copy the contents of register D to the
// memory segment address stored in RAM[13]
void CodeWriter::copyRegisterDToAddressStoredInRam(int address)
{
    asmFile << "@R" << address << "\n"
            << "A=M\n"
            << "M=D\n";
}

// Writes the assembly code to the output file that will copy the contents of a register to
// another register
void CodeWriter::setRegisterDEqualToRegister(std::string registerAorM)
{
    asmFile << "D=" << registerAorM << "\n";
}

// Writes the assembly code to the output file that will load a constant value into register A
void CodeWriter::loadConstantValueIntoRegisterA(int index)
{
    asmFile << "@" << index << "\n";
}

// Writes the assembly code to the output file that will load R0-R15 into register A
void CodeWriter::loadRamAddressIntoRegisterA(int address)
{
    asmFile << "@R" << address << "\n";
}

// Writes the assembly code to the output file that will load the memory address of a static
// variable into register A
void CodeWriter::loadAddressOfStaticVariableIntoRegisterA(int index)
{
    std::string filename = currentFile.substr(0, currentFile.find('.') + 1);
    while (filename.find('\\') != -1) {
        filename = filename.substr(filename.find('\\') + 1, std::string::npos);
    }
    asmFile << "@" << filename << index << "\n";
}

// Writes the assembly code to the output file that will repeat the callback function x times
void CodeWriter::loopXTimes(int x, std::function<void()> codeBlockToLoopOver)
{
    asmFile << "@" << x << "\n"
            << "D=A\n"
            << "@COUNTER" << labelCounter << "\n"
            << "M=D\n"
            << "(LOOP" << labelCounter << ")\n";
    codeBlockToLoopOver();
    asmFile << "@COUNTER" << labelCounter << "\n"
            << "M=M-1\n"
            << "D=M\n"
            << "@LOOP" << labelCounter << "\n"
            << "D;JGT\n";
    ++labelCounter;
}

// Writes...
void CodeWriter::jumpToAddressStoredInRam(std::string address)
{
    copyRamToRegisterD(address);
    asmFile << "A=D\n"
            << "0;JMP\n";
}

//
void CodeWriter::incrementValueInRegisterD()
{
    asmFile << "D=D+1\n";
}
