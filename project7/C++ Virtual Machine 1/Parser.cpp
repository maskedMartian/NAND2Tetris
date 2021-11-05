#include "Parser.h"

#include <algorithm>
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

bool Parser::hasMoreCommands()
{
    std::string line;
    std::streampos position;

    do {
        position = vmFile.tellg();
        std::getline(vmFile, line);
    } while ((isBlank(line) || isComment(line)) && !vmFile.eof());
    if (!vmFile.eof()) {
        vmFile.seekg(position);
        return true;
    } else {
        if (!(isBlank(line) || isComment(line))) {
            vmFile.seekg(position);
            return true;
        } else {
            return false;
        }
    }
}

void Parser::advance()
{
    if (hasMoreCommands()) {
        std::getline(vmFile, command);
    } // I could potentially set command to some arbitrary pre-defined value
    /*
    * Not sure if I need this or not - I don't think it will matter, but we'll see
    // truncates end of line comment
    if (command.find("//") != std::string::npos) {
        command = command.substr(0, command.find("//"));
    }
    */
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

bool Parser::isBlank(std::string line) const
{
    if (std::all_of(line.begin(), line.end(), isspace)) {
        return true;
    } else {
        return false;
    }
}

bool Parser::isComment(std::string line) const
{
    std::string partial;
    if (line.find("//") != std::string::npos) {
        partial = line.substr(0, line.find("//"));
        if (std::all_of(partial.begin(), partial.end(), isspace)) {
            return true;
        }
    }
    return false;
}
