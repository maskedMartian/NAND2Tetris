#pragma once

#include <fstream>
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
    void setFileName(std::string filename);
    void writeArithmetic(std::string command);
    void WritePushPop(CommandTypes command, std::string segment, int index);
    void close();
private:
    void popStackToRegisterM();
    void popStackToRegisterD();
    void pushRegisterDToStack();
    void compareRegistersMAndD(std::string command);
    void addEndOfProgramCode();
    void loadSegmentAddressIntoRegisterA(std::string segment, int index);
    void copyAddressFromRegisterAToR13();
    void copyRegisterDToAddressStoredInR13();
    void setRegisterDEqualToRegister(std::string register);
    void loadConstantValueIntoRegisterA(int index);
    void loadRAMAddressIntoRegisterA(int address);
    void loadAddressOfStaticVariableIntoRegisterA(int index);
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
