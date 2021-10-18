#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <string>

class Parser
{
public:
    Parser(std::string filename);
    bool hasMoreCommands();
    void advance();
    std::string commandType() const;
    std::string symbol() const;
    std::string dest() const;
    std::string comp() const;
    std::string jump() const;
    bool isConstant() const;
    std::string getAddress() const;
    std::string getConstant() const;
    void resetFile();
    void closeFile();
private:
    bool isBlank(std::string line) const;
    bool isComment(std::string line) const;
    std::ifstream asmFile;
    std::string command;
    int lineAddress;
};

#endif
