#include "Parser.h"

#include <stdlib.h>
#include <iostream>

Parser::Parser(std::string fileName) : asmFile{ fileName }
{
    /*
    if (fileName.substr(fileName.length() - 4, 4) != ".asm")
    {
        std::cout << "ERROR: Assembly file expected";
        exit(1);
    }
    */
}

bool Parser::hasMoreCommands()
{
    return (!asmFile.eof()) ? true : false;
}

void Parser::advance()
{
    std::getline(asmFile, command);
    std::cout << command << "\n";
}

std::string Parser::commandType()
{
    return "foo";
}

std::string Parser::symbol()
{
    return "foo";
}

std::string Parser::dest()
{
    return "foo";
}

std::string Parser::comp()
{
    return "foo";
}

std::string Parser::jump()
{
    return "foo";
}

bool Parser::isConstant()
{
    return true;
}

std::string Parser::getAddress()
{
    return "foo";
}

std::string Parser::getConstant()
{
    return "foo";
}

void Parser::resetFile()
{
}

void Parser::closeFile()
{
    asmFile.close();
}

