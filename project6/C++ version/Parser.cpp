#include "Parser.h"

#include <algorithm>
#include <istream>
#include <iostream>

Parser::Parser(std::string fileName) : lineAddress{ 0 }
{
    if (fileName.substr(fileName.length() - 4, 4) != ".asm") {
        std::cout << "ERROR: Assembly file expected\n";
        exit(1);
    } else {
        asmFile.open(fileName);
        if (asmFile.fail()) {
            std::cout << "ERROR: Something went wrong trying to open the necessary files\n";
            exit(1);
        }
    }
}

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
    // REMOVE THIS LINE WHEN FINISHED DEBUGGING!
    std::cout << "advance:  " << command << "\n";
}

std::string Parser::commandType() const
{
    if (command[0] == '@') {
        return "A_COMMAND";
    } else if (command[0] == '(' && command[command.length() - 1] == ')') {
        return "L_COMMAND";
    } else {
        return "C_COMMAND";
    }
}

std::string Parser::symbol() const
{
    if (command[0] == '@') {
        return command.substr(1, command.length() - 1);
    } else {
        return command.substr(1, command.length() - 2);
    }
}

std::string Parser::dest()
{
    if (command.find('=') != std::string::npos) {
        return command.substr(0, command.find('='));
    } else {
        return "";
    }
}

std::string Parser::comp()
{
    if (command.find('=') != std::string::npos) {
        return command.substr(command.find('='), command.length() - 1);
    } else {
        return command.substr(0, command.find(';'));
    }

    // TRY THIS:
    // if (command.find('=') != std::string::npos) {
    //     return command.substr(command.find('='), command.length() - 1);
    // } else if (command.find(';') != std::string::npos) {
    //     return command.substr(0, command.find(';'));
    // } else {
    //     return "";
    // }
}

std::string Parser::jump()
{
    if (command.find(';') != std::string::npos) {
        return command.substr(command.find(';') + 1, command.length() - 1);
    } else {
        return "";
    }
}

bool Parser::isConstant()
{
    std::string s = symbol();

    if (std::all_of(s.begin(), s.end(), isdigit)) {
        return true;
    } else {
        return false;
    }
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
    asmFile.clear();
    asmFile.seekg(0, asmFile.beg);
}

void Parser::closeFile()
{
    asmFile.close();
}


bool Parser::isBlank(std::string line)
{
    if (std::all_of(line.begin(), line.end(), isspace)) {
        return true;
    } else {
        return false;
    }
}

bool Parser::isComment(std::string line)
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
