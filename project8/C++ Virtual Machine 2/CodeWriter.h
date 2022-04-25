#pragma once

#include <fstream>
#include <functional>
#include <map>
#include <string>

#include "CommandTypes.h"

#include <iomanip>

// The CodeWriter class translates VM commands into HACK assembly code and writes it to an assembly
// file
class CodeWriter
{
public:
    CodeWriter(std::string filename);
    ~CodeWriter();
    void setFileName(const std::string& filename);
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

    void writeSpy(std::string commandPhrase)
    {
        spy << std::setw(22) << std::left << commandPhrase << "  " << lineCount1 << "  " << lineCount2 << "\n";
    }
private:
    void popStackToRegisterM();
    void popStackToRegisterD();
    void popStackToRamAddress(std::string address);
    void pushRegisterDToStack();
    void pushRegisterMToStack();
    void pushRamAddressToStack(std::string address);
    void copyRegisterDToRamAddress(std::string address);
    void copyRamAddressToRegisterD(std::string address);
    void copyFromRamAddressToRamAddress(std::string fromAddress, std::string toAddress);
    void copyFromRamAddressToRamAddress(std::string fromAddress, int fromAddressIndex, std::string toAddress);
    void compareRegistersMAndD(std::string command);
    void addEndOfProgramCode();
    void loadRamSegmentAddressIntoRegisterA(std::string segment, int index, bool test = false);
    void copyRegisterAToRamAddress(std::string address);
    void copyRegisterDToAddressStoredInRam(std::string address);
    void setRegisterDEqualTo(std::string registerAorM);
    void loadValueIntoRegisterA(int index);
    void loadAddressOfStaticVariableIntoRegisterA(int index);
    void loopXTimes(int x, std::function<void()> codeBlockToLoopOver);
    void jumpToAddressStoredInRam(std::string address);
    void jumpToFunction(std::string functionName);
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
    int lineCount1;
    int lineCount2;
    std::ofstream spy;
};
