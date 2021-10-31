#include "Parser.h"

Parser::Parser()
{

}

bool Parser::hasMoreCommands(/*input file stream*/)
{
    return false;
}

void Parser::advance()
{

}

commandTypes Parser::commandType()
{
    return C_IF;
}

std::string Parser::arg1()
{
    return "foo";
}

std::string Parser::arg2()
{
    return "foo";
}
