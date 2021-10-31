#pragma once

/*
FROM THE BOOK:
The Parser class handles the parsing of a single .vm file, and encapsulates
access to the input code. It reads VM commands, parses them, and provides
convenient access to their components. In addition, it removes all white
space and comments.
*/

class Parser
{
    Parser();
    bool hasMoreCommands(/*input file stream*/);
    void advance();
    std::string commandType();  // USE AN ENUM FOR THIS???
    std::string arg1();
    std::string arg2();
};
