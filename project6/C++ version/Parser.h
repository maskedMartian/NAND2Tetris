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
    std::string commandType() const;
    std::string symbol() const;
    std::string dest();
    std::string comp();
    std::string jump();
    bool isConstant();
    std::string getAddress();
    std::string getConstant();
    void resetFile();
    void closeFile();
private:
    bool isBlank(std::string line);
    bool isComment(std::string line);
    std::ifstream asmFile;
    //std::string command;
    int lineAddress;
};

#endif

