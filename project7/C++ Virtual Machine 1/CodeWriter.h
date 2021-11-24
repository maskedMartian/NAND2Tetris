#pragma once

#include <string>

enum class PushPopCommands
{
    c_push,
    c_pop
};

#define C_PUSH PushPopCommands::c_push
#define C_POP  PushPopCommands::c_pop

// The CodeWriter class translates VM commands into HACK assembly code
class CodeWriter
{
public:
    CodeWriter(/*output file stream*/);
    void setFileName(std::string fileName);
    void writeArithmetic(std::string command);
    void WritePushPop(PushPopCommands command, std::string segment, int index);
    void close();
private:

};
