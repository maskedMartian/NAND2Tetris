#pragma once

#include <fstream>
#include <string>

#include "CommandTypes.h"

#define EXTENSION_LENGTH 3

// The Parser class handles the parsing of a single .vm file, and encapsulates access to the input
// code. It reads VM commands, parses them, and provides convenient access to their components. In
// addition, it removes all white space and comments.
class Parser
{
public:
    Parser(std::string filename);
    ~Parser();
    bool theFileHasMoreCommands();
    void advance();
    CommandTypes commandType() const;
    std::string arg1() const;
    int arg2() const;
private:
    bool isBlank(std::string line) const;
    bool isComment(std::string line) const;
    std::string extractFromCommandPhrase(int wordPosition) const;
    std::ifstream vmFile;
    std::string commandPhrase;
};
