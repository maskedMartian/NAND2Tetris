#pragma once

#include <fstream>
#include <string>

/*
FROM THE BOOK:
The Parser class handles the parsing of a single .vm file, and encapsulates
access to the input code. It reads VM commands, parses them, and provides
convenient access to their components. In addition, it removes all white
space and comments.
*/

enum class commandTypes
{
    c_arithmetic,
    c_push,
    c_pop,
    c_label,
    c_goto,
    c_if,
    c_function,
    c_return,
    c_call,
};

#define C_ARITHEMTIC commandTypes::c_arithmetic
#define C_PUSH       commandTypes::c_push
#define C_POP        commandTypes::c_pop
#define C_LABEL      commandTypes::c_label
#define C_GOTO       commandTypes::c_goto
#define C_IF         commandTypes::c_if
#define C_FUNCTION   commandTypes::c_function
#define C_RETURN     commandTypes::c_return
#define C_CALL       commandTypes::c_call

class Parser
{
public:
    Parser(std::string filename);
    ~Parser();
    bool hasMoreCommands();
    void advance();
    commandTypes commandType();
    std::string arg1();
    std::string arg2();
private:
    bool isBlank(std::string line) const;
    bool isComment(std::string line) const;
    std::ifstream vmFile;
};
