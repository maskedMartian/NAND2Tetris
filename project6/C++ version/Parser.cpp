#include "Parser.h"

#include <algorithm>
#include <bitset>
#include <iostream>

Parser::Parser(std::string filename) : lineAddress{ 0 }
{
    if (filename.substr(filename.length() - 4, 4) != ".asm") {
        std::cout << "ERROR: Assembly file expected\n";
        exit(1);
    } else {
        asmFile.open(filename);
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
    // remove all spaces from the string
    command.erase(std::remove_if(command.begin(), command.end(), isspace), command.end());
    if (commandType() == "A_COMMAND" || commandType() == "C_COMMAND") {
        lineAddress++;
    }
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

std::string Parser::dest() const
{
    if (command.find('=') != std::string::npos) {
        return command.substr(0, command.find('='));
    } else {
        return "";
    }
}

std::string Parser::comp() const
{
    if (command.find('=') != std::string::npos) {
        return command.substr(command.find('=') + 1, command.length() - 1);
    } else if (command.find(';') != std::string::npos) {
        return command.substr(0, command.find(';'));
    } else {
        return "";
    }
}

std::string Parser::jump() const
{
    if (command.find(';') != std::string::npos) {
        return command.substr(command.find(';') + 1, command.length() - 1);
    } else {
        return "";
    }
}

bool Parser::isConstant() const
{
    std::string s = symbol();

    if (std::all_of(s.begin(), s.end(), isdigit)) {
        return true;
    } else {
        return false;
    }
}

std::string Parser::getAddress() const
{
    if (commandType() == "L_COMMAND") {
        return std::bitset<16>(lineAddress).to_string();
    } else {
        return "";
    }
}

std::string Parser::getConstant() const
{
    if (commandType() == "A_COMMAND") {
        return std::bitset<16>(stoi(symbol(), nullptr)).to_string();
    } else {
        return "";
    }
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
