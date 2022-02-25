#pragma once

#include <fstream>
#include <functional>
#include <map>
#include <string>

#include "CommandTypes.h"

// The CodeWriter class translates VM commands into HACK assembly code and writes it to an assembly
// file
class CodeWriter
{
public:
    CodeWriter(std::string filename);
    ~CodeWriter();
    void writeArithmetic(std::string command);
    void writePushPop(CommandTypes command, std::string segment, int index);
    void close();
    void writeInit();
    void writeLabel(std::string label);
    void writeGoto(std::string label);
    void writeIf(std::string label);
    void writeCall(std::string functionName, int numArgs);
    void writeReturn();
    void writeFunction(std::string functionName, int numLocals);
private:
    void popStackToRegisterM();
    void popStackToRegisterD();
    void popStackToRam(std::string address);
    void pushRegisterDToStack();
    void pushRegisterMToStack();
    void copyRegisterDToRam(std::string address);
    void copyRamToRegisterD(std::string address);
    void copyRamToRam(std::string address1, std::string address2);
    void compareRegistersMAndD(std::string command);
    void addEndOfProgramCode();
    void loadSegmentAddressIntoRegisterA(std::string segment, int index);
    void copyRegisterAToRam(std::string address);
    void copyRegisterDToAddressStoredInRam(std::string address);
    void setRegisterDEqualToRegister(std::string registerAorM);
    void loadValueIntoRegisterA(int index);
    // void loadRamAddressIntoRegisterA(int address);
    void loadAddressOfStaticVariableIntoRegisterA(int index);
    void loopXTimes(int x, std::function<void()> codeBlockToLoopOver);
    void jumpToAddressStoredInRam(std::string address);
    void incrementStackPointer();
    void decrementStackPointer();
    std::string currentFile;
    std::ofstream asmFile;
    int labelCounter = 0;
    enum ops{ add, sub, eq, gt, lt, AND, OR };
    std::map<std::string, ops> operation = {
        { "add", add },
        { "sub", sub },
        { "eq", eq },
        { "lt", lt },
        { "gt", gt },
        { "and", AND },
        { "or", OR }
    };
    enum segs{ argument, local, STATIC, constant, THIS, that, pointer, temp };
    std::map<std::string, segs> segments = {
        { "argument", argument },
        { "local", local },
        { "static", STATIC },
        { "constant", constant },
        { "this", THIS },
        { "that", that },
        { "pointer", pointer },
        { "temp", temp }
    };
};
