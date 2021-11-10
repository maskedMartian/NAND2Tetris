#include "Parser.h"

#include <algorithm>
#include <iostream>

// Opens the input file/stream and gets ready to parse it
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

// Closes the input file
Parser::~Parser()
{
    if (vmFile.is_open()) {
        vmFile.close();
    }
}

// Checks if there are more commands in the input stream, skipping over all blank lines and lines
// containing only comments
bool Parser::theFileHasMoreCommands()
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

// Reads the next command from the input and makes it the current command
void Parser::advance()
{
    std::string com1, com2, com3;
    if (!theFileHasMoreCommands()) {
        _commandType = C_NONE;
        _arg1 = "";
        _arg2 = -1;
        return;
    }

    if (theFileHasMoreCommands()) {
        std::getline(vmFile, command);

        // truncates end of line comment
        if (command.find("//") != std::string::npos) {
            command = command.substr(0, command.find("//"));
        }




        /*
        // remove leading whitespace
        command = command.substr(command.find_first_not_of(" "));
        // remove trailing whitespace
        command = command.substr(0, command.find_last_not_of(" ") + 1);
        com1 = command.substr(0,command.find(" "));
        command = command.substr(com1.length(), command.find(" "));
        */
    } else {
        _commandType = C_NONE;
        _arg1 = "";
        _arg2 = -1;
    }


    /*
    // remove all spaces from the string
    command.erase(std::remove_if(command.begin(), command.end(), isspace), command.end());
    */
}

// Returns the type of the current VM command
commandTypes Parser::commandType() const
{
    return _commandType;
}

// Returns the first argument of the current command. In the case of C_ARITHMETIC, the command
// itself (add, sub,etc.) is returned. Should not be called if the current command is C_RETURN.
std::string Parser::arg1() const
{
    if (_commandType != C_RETURN) {
        return _arg1;
    } else {
        exit(1); // return NULL; instead?
    }
}

// Returns the second argument of the current command. Should be called only if the current command
// is C_PUSH, C_POP, C_FUNCTION, or C_CALL.
int Parser::arg2() const
{
    switch (_commandType) {
    case C_PUSH:
    case C_POP:
    case C_FUNCTION:
    case C_CALL:
        return _arg2;
    default:
        exit(1); // return NULL; instead?
    }
}

// Checks whether its input string is completely whitespace
bool Parser::isBlank(std::string line) const
{
    if (std::all_of(line.begin(), line.end(), isspace)) {
        return true;
    } else {
        return false;
    }
}

// Checks whether its input string is just a comment without any other text on that line
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
