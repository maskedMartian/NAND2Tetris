#include "Parser.h"

#include <algorithm>
#include <iostream>

#define ERROR                 1
#define FIRST_WORD            1
#define SECOND_WORD           2
#define THIRD_WORD            3

// Opens the input file/stream and gets ready to parse it
Parser::Parser(std::string filename)
{
    // if the file extension is not .vm
    if (filename.substr(filename.length() - CHARS_IN_EXTENSION, CHARS_IN_EXTENSION) != ".vm") {
        std::cout << "ERROR: VM file expected\n";
        exit(ERROR);
    } else {
        vmFile.open(filename);
        if (vmFile.fail()) {
            std::cout << "ERROR: Something went wrong trying to open the necessary files\n";
            exit(ERROR);
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

    // skip over all blank lines and comment only lines
    do {
        position = vmFile.tellg();
        std::getline(vmFile, line);
    } while ((isBlank(line) || isComment(line)) && !vmFile.eof());

    // if this is not the last line of the file
    if (!vmFile.eof()) {
        vmFile.seekg(position);
        return true;
    //this is the last line of the file
    } else {
        // if the last line of the file is a valid command
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
        commandPhrase = "none";
    }
}

// Returns the type of the current VM command
CommandTypes Parser::commandType() const
{
    const std::string firstWord = extractFromCommandPhrase(FIRST_WORD);
    
    if (firstWord == "push") return C_PUSH;
    if (firstWord == "pop") return C_POP;
    if (firstWord == "label") return C_LABEL;
    if (firstWord == "goto") return C_GOTO;
    if (firstWord == "if") return C_IF;
    if (firstWord == "function") return C_FUNCTION;
    if (firstWord == "return") return C_RETURN;
    if (firstWord == "call") return C_CALL;
    if (firstWord == "none") return C_NONE;
    return C_ARITHMETIC;
}

// Returns the first argument of the current command. In the case of C_ARITHMETIC, the command
// itself (add, sub, etc.) is returned. Should not be called if the current command is C_RETURN.
std::string Parser::arg1() const
{
    if (commandType() == C_ARITHMETIC) return extractFromCommandPhrase(FIRST_WORD);
    if (commandType() == C_RETURN || commandType() == C_NONE) return "";
    return extractFromCommandPhrase(SECOND_WORD);
}

// Returns the second argument of the current command. Should be called only if the current command
// is C_PUSH, C_POP, C_FUNCTION, or C_CALL.
int Parser::arg2() const
{
    switch (commandType()) {
    case C_PUSH:
    case C_POP:
    case C_FUNCTION:
    case C_CALL:
        return stoi(extractFromCommandPhrase(THIRD_WORD));
    default:
        return 0;
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
    // if there is a comment on this line
    if (line.find("//") != std::string::npos) {
        // copy all the text to the left of the comment into partial
        const std::string partial = line.substr(0, line.find("//"));
        // if partial is completely whitespace
        if (std::all_of(partial.begin(), partial.end(), isspace)) return true;
    }
    return false;
}

// Returns the specified word from the string stored in the commandPhrase
// member variable
std::string Parser::extractFromCommandPhrase(int positionOfTheWord) const
{
    if (positionOfTheWord < FIRST_WORD || positionOfTheWord > THIRD_WORD) return commandPhrase;

    std::string phrase = commandPhrase;
    std::string word;

    for (auto i = 0; i < positionOfTheWord; i++) {
        // remove leading whitespace
        phrase = phrase.substr(phrase.find_first_not_of(" "));
        // copy leftmost word of command phrase
        word = phrase.substr(0, phrase.find(" "));
        // remove leftmost word from command phrase
        phrase = phrase.substr(word.length());
    }
    return word;
}
