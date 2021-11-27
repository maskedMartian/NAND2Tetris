#pragma once

#include <fstream>
#include <map>
#include <string>

#include "CommandTypes.h"

// The CodeWriter class translates VM commands into HACK assembly code
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
    void popStackToRegisterA();
    void popStackToRegisterD();
    void pushRegisterDToStack();
    std::string currentFile;
    std::ofstream asmFile;
    enum ops { add, sub, neg, eq, gt, lt, NOT, AND, OR };
    std::map<std::string, ops> operation = {
        { "add", add },
        { "sub", sub },
        { "neg", neg },
        { "MD",  eq },
        { "A",   lt },
        { "AM",  gt },
        { "AD",  NOT },
        { "AMD", AND },
        { "AMD", OR }
    };
};
