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
    if (theFileHasMoreCommands()) {
        std::getline(vmFile, commandPhrase);
        // truncate end of line comment
        if (commandPhrase.find("//") != std::string::npos) {
            commandPhrase = commandPhrase.substr(0, commandPhrase.find("//"));
        }
    } else {
        commandPhrase = "";
    }
}

// Returns the type of the current VM command
commandTypes Parser::commandType()
{
    std::string firstWord = returnWordFromCommandPhrase(1);
    
    if (firstWord == "push") return C_PUSH;
    if (firstWord == "pop") return C_POP;
    if (firstWord == "label") return C_LABEL;
    if (firstWord == "goto") return C_GOTO;
    if (firstWord == "if") return C_IF;
    if (firstWord == "function") return C_FUNCTION;
    if (firstWord == "return") return C_RETURN;
    if (firstWord == "call") return C_CALL;
    return C_ARITHMETIC;
}

// Returns the first argument of the current command. In the case of C_ARITHMETIC, the command
// itself (add, sub,etc.) is returned. Should not be called if the current command is C_RETURN.
std::string Parser::arg1()
{
    if (commandType() == C_RETURN) exit(1);
    if (commandType() == C_ARITHMETIC) return returnWordFromCommandPhrase(1);
    return returnWordFromCommandPhrase(2);
}

// Returns the second argument of the current command. Should be called only if the current command
// is C_PUSH, C_POP, C_FUNCTION, or C_CALL.
int Parser::arg2() const
{
    /*
    switch (_commandType) {
    case C_PUSH:
    case C_POP:
    case C_FUNCTION:
    case C_CALL:
        return _arg2;
    default:
        exit(1); // return NULL; instead?
    }
    */
    return 0;
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

std::string Parser::returnWordFromCommandPhrase(int number)
{
    std::string leftover = commandPhrase;
    std::string word;

    if (number < 1 || number > 3) return leftover;

    for (auto i = 0; i < number; i++) {
        leftover = leftover.substr(leftover.find_first_not_of(" "));
        word = leftover.substr(0, leftover.find(" "));
        leftover = leftover.substr(word.length());
    }
    return word;
}
