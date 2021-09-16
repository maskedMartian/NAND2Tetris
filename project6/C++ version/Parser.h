#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <string>

class Parser
{
public:
    std::string command;
    Parser(std::string fileName);
    bool hasMoreCommands();
    void advance();
    std::string commandType();
    std::string symbol();
    std::string dest();
    std::string comp();
    std::string jump();
    bool isConstant();
    std::string getAddress();
    std::string getConstant();
    void resetFile();
    void closeFile();
private:
    std::ifstream asmFile;
    //std::string command;
    int lineAddress;
};

#endif

