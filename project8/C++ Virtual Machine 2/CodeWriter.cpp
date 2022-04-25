#include <cstdlib>

#include "CodeWriter.h"

// Opens the output file/stream and gets ready to write into it
CodeWriter::CodeWriter(std::string filename) : 
    currentFile{ filename }
{
    //currentFile = filename.substr(0, filename.find('.')) + ".asm";
    //asmFile.open(currentFile);
    asmFile.open(filename.substr(0, filename.find('.')) + ".asm");

    spy.open("spy.txt");
    lineCount1 = 0;
    lineCount2 = 0;
}

// Closes the input file
CodeWriter::~CodeWriter()
{
    if (asmFile.is_open()) {
        addEndOfProgramCode(); // FAIRLY CERTAIN I don't need this line of code at all because the required infinite loop is build into the vm code
        asmFile.close();
    }

    spy.close();
}

// Informs the code writer that the translation of a new VM file is started
void CodeWriter::setFileName(const std::string& filename)
{
    currentFile = filename;
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
        ++lineCount1;
        lineCount2 += 1;
    // binary operations
    } else {
        popStackToRegisterM();
        switch (operation[command]) {
        case add:
            asmFile << "D=D+M\n";
        ++lineCount1;
        lineCount2 += 1;
            break;
        case sub:
            asmFile << "D=M-D\n";
        ++lineCount1;
        lineCount2 += 1;
            break;
        case AND:
            asmFile << "D=D&M\n";
        ++lineCount1;
        lineCount2 += 1;
            break;
        case OR:
            asmFile << "D=D|M\n";
        ++lineCount1;
        lineCount2 += 1;
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
    // refactor to use an if instead of a switch in this function!
    // RIDICULOUS!!! Here I have the exact same switch statement twice - once at the beginning of each leg of an if-else statement. I should just do the switch once and then do the if as a separate control structure instead of nesting the switch inside the if - CHECK PROJECT 7 (VM1) FOR THE SAME ISSUE
    std::string registerAorM = "M";

    if (command == C_PUSH) {
        // push the value of segment[index] onto the stack
        switch (segments[segment]) {
        case argument:
            loadRamSegmentAddressIntoRegisterA("ARG", index);
            break;
        case local:
            loadRamSegmentAddressIntoRegisterA("LCL", index);
            break;
        case STATIC:
            loadAddressOfStaticVariableIntoRegisterA(index);
            break;
        case constant:
            loadValueIntoRegisterA(index);
            registerAorM = "A";
            break;
        case THIS:
            loadRamSegmentAddressIntoRegisterA("THIS", index);
            break;
        case that:
            loadRamSegmentAddressIntoRegisterA("THAT", index);
            break;
        case pointer:
            loadRamSegmentAddressIntoRegisterA("R3", index, true);
            break;
        case temp:
            loadRamSegmentAddressIntoRegisterA("R5", index, true);
            break;
        }
        setRegisterDEqualTo(registerAorM); // Change this by making two functions: setRegisterDEqualToRegisterA() and setRegisterDEqualToRegisterM() and choose between them with the variable registerAorM
        pushRegisterDToStack();
    // (command == C_POP)
    } else {
        // pop the stack and store the value in segment[index]
        switch (segments[segment]) {
        case argument:
            loadRamSegmentAddressIntoRegisterA("ARG", index);
            break;
        case local:
            loadRamSegmentAddressIntoRegisterA("LCL", index);
            break;
        case STATIC:
            loadAddressOfStaticVariableIntoRegisterA(index);
            break;
        case THIS:
            loadRamSegmentAddressIntoRegisterA("THIS", index);
            break;
        case that:
            loadRamSegmentAddressIntoRegisterA("THAT", index);
            break;
        case pointer:
            loadRamSegmentAddressIntoRegisterA("R3", index, true);
            break;
        case temp:
            loadRamSegmentAddressIntoRegisterA("R5", index, true);  // <<=== I GUARANTEE THIS IS WHERE THE PROBLEM LIES!!!!!!
            break;
        }
        copyRegisterAToRamAddress("R13");
        popStackToRegisterD();
        copyRegisterDToAddressStoredInRam("R13");
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
    // load RAM[256] into the stack pointer - SP or R0 or RAM[0]
    asmFile << "@256\n"
            << "D=A\n"
            << "@SP\n"
            << "M=D\n";
        lineCount1 += 4;
        lineCount2 += 4;
    // jumpToFunction("Sys.init"); -- REMOVE THIS
    // call Init.sys
    writeCall("Sys.init", 0);
}

// Writes assembly code to the assembly file that effects the label command.
void CodeWriter::writeLabel(std::string label)
{
    asmFile << "(" << label << ")\n";
    lineCount2 += 1;
}

// Writes assembly code to the assembly file that effects the goto command.
void CodeWriter::writeGoto(std::string label)
{
    asmFile << "@" << label << "\n"
            << "0;JMP" << "\n";
        lineCount1 += 2;
        lineCount2 += 2;
}

// Writes assembly code to the assembly file that effects the if-goto command.
void CodeWriter::writeIf(std::string label)
{
    popStackToRegisterD();
    asmFile << "@" << label << "\n"
            << "D;JNE" << "\n";
        lineCount1 += 2;
        lineCount2 += 2;
}

// Writes assembly code to the assembly file that effects the call command.
void CodeWriter::writeCall(std::string functionName, int numArgs)
{
    asmFile << "@SP\n"
            << "D=M\n";
        lineCount1 += 2;
        lineCount2 += 2;
    // OPTIMIZATION - here, instead of looping through and printing D=D-1 numArgs times, load numArgs into A and subtract A from D:
    // @numArgs
    // D=D-A
    for (auto i = 0; i < numArgs; ++i) {
        asmFile << "D=D-1\n";
        ++lineCount1;
        lineCount2 += 1;
    }
    copyRegisterDToRamAddress("R14");
    // -------------------------------------------
    asmFile << "@RETURN" << labelCounter << "\n" // push return-address // (Using the label declared below)
            << "D=A\n";
        lineCount1 += 2;
        lineCount2 += 2;
    pushRegisterDToStack();
    // -------------------------------------------
    pushRamAddressToStack("LCL"); // Save LCL of the calling function - NOT RIGHT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! (DOESN'T MATCH TEST VM CODE BUT TEST CODE IS SETUP DIFFERENT SO MISMATCH IS NOT IMPORTANT)
    pushRamAddressToStack("ARG"); // Save ARG of the calling function - NOT RIGHT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    pushRamAddressToStack("THIS"); // Save THIS of the calling function
    pushRamAddressToStack("THAT"); // Save THAT of the calling function
    // I could use my new overload of copyFromRamAddressToRamAddress() to do the following line and eliminate the code at the top of this function.
    copyFromRamAddressToRamAddress("R14", "ARG");// ARG =  SP-n-5 // Reposition ARG (n = number of args)
    copyFromRamAddressToRamAddress("SP", "LCL"); // LCL = SP // Reposition LCL
    jumpToFunction(functionName); // goto f // Transfer control
    writeLabel("RETURN" + std::to_string(labelCounter)); // Declare a label for the return-address
    ++labelCounter;
}

// Writes assembly code to the assembly file that effects the return command.
void CodeWriter::writeReturn()
{
    const std::string FRAME = "R14",
                      RET = "R15";

    copyFromRamAddressToRamAddress("LCL", FRAME); // FRAME = LCL
    copyFromRamAddressToRamAddress(FRAME, -5, RET); // RET = *(FRAME-5)
    writePushPop(C_POP, "argument", 0); // *ARG = pop()
    copyFromRamAddressToRamAddress("ARG", "SP"); incrementStackPointer(); // SP = ARG + 1
    copyFromRamAddressToRamAddress(FRAME, -1, "THAT"); // THAT = *(FRAME-1)
    copyFromRamAddressToRamAddress(FRAME, -2, "THIS"); // THIS = *(FRAME-2)
    copyFromRamAddressToRamAddress(FRAME, -3, "ARG"); // ARG = *(FRAME-3)
    copyFromRamAddressToRamAddress(FRAME, -4, "LCL"); // LCL = *(FRAME-4)
    jumpToAddressStoredInRam(RET);// goto RET

#if 0
    // copy the previous value of the stack pointer into R14 for temporary storage
    copyFromRamAddressToRamAddress("ARG", "R14");
    // pop the function return value off the stack and into the memory address that will be the top
    // of the stack after return
    writePushPop(C_POP, "argument", 0); // --- INSTEAD OF POPPING THE RETURN VALUE OFF THE STACK AND DIRECTLY INTO THE PLACE WHERE IT WILL
    // BE ON RETURN, POP IT INTO TEMPORARY STORAGE
    // BECAUSE:
    // if there are no arguments pushed to the stack before the function call (i.e. the function has no parameters - such as a getter function)
    // then putting the return value where is will be actually copies it over top of the return address of the function call
    // copy the base address of the local segment into the stack pointer
    copyFromRamAddressToRamAddress("LCL", "SP");
    // pop the stored frame from the stack and restore caller's environment
    popStackToRamAddress("THAT");
    popStackToRamAddress("THIS");
    popStackToRamAddress("ARG");
    popStackToRamAddress("LCL");
    // pop the stored return address into R15 for temporary storage
    popStackToRamAddress("R15"); // IF THERE ARE NO ARGUMENTS TO THE FUNCTION THEN AT THIS POINT THE RETURN ADDRESS HAS HAD THE RETURN VALUE COPIED OVER IT!!!!!!!!!!!!!!!!!!!!!
    // copy the address temporarily stored in R14 to the stack pointer
    copyFromRamAddressToRamAddress("R14", "SP");
    // increment the stack pointer so it points to the address just above the address containing the
    // return value 
    incrementStackPointer();
    // return to the next line immediately following the function call
    jumpToAddressStoredInRam("R15");
#endif
}

// Writes assembly code to the assembly file that effects the function command.
void CodeWriter::writeFunction(std::string functionName, int numLocals)
{
    // NOW I'M CURIOUS WHETHER THE LAMBDA IS NECESSARY WHEN USING STD::FUNCTION - COULD I USE std::function<void(std::string, int)> ???? 
    writeLabel(functionName);
    loopXTimes(numLocals, [this]() {
        writePushPop(C_PUSH, "constant", 0);
    });
}

// Writes the assembly code to the output file that will pop a value from the stack into
// register M
void CodeWriter::popStackToRegisterM()
{
    decrementStackPointer();
    asmFile << "A=M\n";
        ++lineCount1;
        lineCount2 += 1;
}

// Writes the assembly code to the output file that will pop a value from the stack into
// register D
void CodeWriter::popStackToRegisterD()
{
    popStackToRegisterM();
    asmFile << "D=M\n";
        ++lineCount1;
        lineCount2 += 1;
}

// Writes the assembly code to the output file that will pop a value from the stack into a
// specified RAM address
void CodeWriter::popStackToRamAddress(std::string address)
{
    popStackToRegisterD();
    copyRegisterDToRamAddress(address);
}

// Writes the assembly code to the output file that will push the value in register D onto the
// stack
void CodeWriter::pushRegisterDToStack()
{
    asmFile << "@SP\n"
            << "A=M\n"
            << "M=D\n";
        lineCount1 += 3;
        lineCount2 += 3;
    incrementStackPointer();
}

// Writes the assembly code to the output file that will push the value in register M onto the
// stack
void CodeWriter::pushRegisterMToStack()
{
    asmFile << "D=M\n";
        lineCount1 += 1;
        lineCount2 += 1;
    pushRegisterDToStack();
}

// Writes the assembly code to the output file that will push the value from a specified RAM
// address onto the stack
void CodeWriter::pushRamAddressToStack(std::string address)
{
    copyRamAddressToRegisterD(address);
    pushRegisterDToStack();
}

// Writes the assembly code to the output file that will copy a value from the D register to a
// specified RAM address
void CodeWriter::copyRegisterDToRamAddress(std::string address)
{
    asmFile << "@" << address << "\n"
            << "M=D\n";
        lineCount1 += 2;
        lineCount2 += 2;
}

// Writes the assembly code to the output file that will copy a value from a specified RAM address
// to the D register
void CodeWriter::copyRamAddressToRegisterD(std::string address)
{
    asmFile << "@" << address << "\n"
            << "D=M\n";
        lineCount1 += 2;
        lineCount2 += 2;
}

// Writes the assembly code to the output file that will copy a value from one specified RAM
// address to another specified RAM address
void CodeWriter::copyFromRamAddressToRamAddress(std::string fromAddress, std::string toAddress)
{
    asmFile << "@" << fromAddress << "\n"
            << "D=M\n"
            << "@" << toAddress << "\n"
            << "M=D\n";
        lineCount1 += 4;
        lineCount2 += 4;
}

// Writes the assembly code to the output file that will copy a value from one specified RAM
// address +/- and index value to another specified RAM address

// CHANGE NAME TO copyFromFRAMEplusIndexToRamAddress OR SOMETHING!!!!
void CodeWriter::copyFromRamAddressToRamAddress(std::string fromAddress,
                                                int fromAddressIndex,
                                                std::string toAddress)
{
    asmFile << "@" << abs(fromAddressIndex) << "\n"
            << "D=A\n"
            << "@" << fromAddress << "\n"
            << "A=M\n"
            << (fromAddressIndex < 0 ? "A=A-D\n" : "A=D+A\n")
            << "D=M\n"
            << "@" << toAddress << "\n"
            << "M=D\n";
        lineCount1 += 7;
        lineCount2 += 7;
}

// Writes the assembly code to the output file that will compare the value in register M to the
// value in register D using the supplied command: equal (eq), less than (lt), or greater than(gt)
void CodeWriter::compareRegistersMAndD(std::string command)
{
    for (auto& ch : command) ch = toupper(ch);
    command = 'J' + command;
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
        lineCount1 += 9;
        lineCount2 += 9;
}

// Writes the assembly code to the output file that will signal the end of the program with an
// infinite loop
void CodeWriter::addEndOfProgramCode()
{
    asmFile << "(END)\n"
            << "@END\n"
            << "0;JMP\n";
        lineCount1 += 3;
        lineCount2 += 3;
}

// Writes the assembly code to the output file that will load the indexed address in the specified
// segment of RAM
void CodeWriter::loadRamSegmentAddressIntoRegisterA(std::string segment, int index, bool test)
{
    // get rid of test - code is correct if test==false
    asmFile << "@" << segment << "\n";
    if (test) {
        asmFile << "D=A\n";
    } else {
        asmFile << "D=M\n";
    }
    asmFile << "@" << index << "\n"
            << "A=D+A\n";
        lineCount1 += 4;
        lineCount2 += 4;
}

// Writes the assembly code to the output file that will copy the segment address from register A
// to RAM[address]
void CodeWriter::copyRegisterAToRamAddress(std::string address)
{
    asmFile << "D=A\n";
        lineCount1 += 1;
        lineCount2 += 1;
    copyRegisterDToRamAddress(address);
}

// Writes the assembly code to the output file that will copy the contents of register D to the
// memory segment address stored in RAM[address]
void CodeWriter::copyRegisterDToAddressStoredInRam(std::string address)
{
    asmFile << "@" << address << "\n"
            << "A=M\n"
            << "M=D\n";
        lineCount1 += 3;
        lineCount2 += 3;
}

// Writes the assembly code to the output file that will copy the contents of a register to
// another register
void CodeWriter::setRegisterDEqualTo(std::string registerAorM)
{
    asmFile << "D=" << registerAorM << "\n";
        lineCount1 += 1;
        lineCount2 += 1;
}

// Writes the assembly code to the output file that will load a constant value into register A
void CodeWriter::loadValueIntoRegisterA(int index)
{
    asmFile << "@" << index << "\n";
        lineCount1 += 1;
        lineCount2 += 1;
}

// Writes the assembly code to the output file that will load the memory address of a static
// variable into register A
void CodeWriter::loadAddressOfStaticVariableIntoRegisterA(int index)
{
    std::string filename = currentFile.substr(0, currentFile.find('.') + 1);
    //  I MIGHT NOT EVEN NEED THE FOLLOWING 3 LINES OF CODE AT ALL
    while (filename.find('\\') != -1) {  // can I use std::string::npos here instead of -1????????????????????????
        filename = filename.substr(filename.find('\\') + 1, std::string::npos);
    }
    asmFile << "@" << filename << index << "\n";
        lineCount1 += 1;
        lineCount2 += 1;
}

// Writes the assembly code to the output file that will repeat the callback function x times
void CodeWriter::loopXTimes(int x, std::function<void()> codeBlockToLoopOver)
{
    if (x == 0) return;
    asmFile << "@" << x << "\n"
            << "D=A\n"
            << "@COUNTER" << labelCounter << "\n"
            << "M=D\n"
            << "(LOOP" << labelCounter << ")\n";
            
        lineCount1 += 5;
        lineCount2 += 5;
    codeBlockToLoopOver();
    asmFile << "@COUNTER" << labelCounter << "\n"
            << "M=M-1\n"
            << "D=M\n"
            << "@LOOP" << labelCounter << "\n"
            << "D;JGT\n";
            
        lineCount1 += 5;
        lineCount2 += 5;
    ++labelCounter;
}

// Writes the assembly code to the output file that will jump the program counter to the specified
// address
void CodeWriter::jumpToAddressStoredInRam(std::string address)
{
    copyRamAddressToRegisterD(address);
    asmFile << "A=D\n"
            << "0;JMP\n";
        lineCount1 += 2;
        lineCount2 += 2;
}

// Writes the assembly code to the output file that will transfer program control to the specified
// function
void CodeWriter::jumpToFunction(std::string functionName)
{
    asmFile << "@" << functionName << "\n"
            << "0;JMP\n";
        lineCount1 += 2;
        lineCount2 += 2;
}

// Writes the assembly code to the output file that will increment the stack pointer
void CodeWriter::incrementStackPointer()
{
    asmFile << "@SP\n"
            << "M=M+1\n";
        lineCount1 += 2;
        lineCount2 += 2;
}

// Writes the assembly code to the output file that will decrement the stack pointer
void CodeWriter::decrementStackPointer()
{
    asmFile << "@SP\n"
            << "M=M-1\n";
        lineCount1 += 2;
        lineCount2 += 2;
}
