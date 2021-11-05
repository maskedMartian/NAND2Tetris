#include "Parser.h"

#include <iostream>

Parser::Parser(std::string filename)
{
    if (filename.substr(filename.length() - 3, 3) != ".vm") {
        std::cout << "ERROR: VM file expected\n";
        exit(1);
    }
    else {
        vmFile.open(filename);
        if (vmFile.fail()) {
            std::cout << "ERROR: Something went wrong trying to open the necessary files\n";
            exit(1);
        }
    }
}

Parser::~Parser()
{
    if (vmFile.is_open()) {
        vmFile.close();
    }
}

/*
bool Parser::hasMoreCommands()
{
    std::string line;
    std::streampos position;

    do {
        position = asmFile.tellg();
        std::getline(asmFile, line);
    } while (!asmFile.eof() && isBlank(line) || isComment(line));
    if (!asmFile.eof()) {
        asmFile.seekg(position);
        return true;
    } else {
        if (!isBlank(line) && !isComment(line)) {
            asmFile.seekg(position);
            return true;
        } else {
            return false;
        }
    }
}
*/
bool Parser::hasMoreCommands()
{
    return false;
}

/*
void Parser::advance()
{
    std::getline(asmFile, command);
    if (command.find("//") != std::string::npos) {
        command = command.substr(0, command.find("//"));
    }
    command.erase(std::remove_if(command.begin(), command.end(), isspace), command.end());
    if (commandType() == "A_COMMAND" || commandType() == "C_COMMAND") {
        lineAddress++;
    }
}
*/
void Parser::advance()
{

}

/*
* Stuff
* More stuff
* Even more stuff
* There is so much stuff!
* Where did you get all this stuff?
*/
commandTypes Parser::commandType()
{
    return C_IF;
}

// WTF does this do?
std::string Parser::arg1()
{
    return "foo";
}

// I don't know, but I bet this does something similar!
std::string Parser::arg2()
{
    return "bar";
}
