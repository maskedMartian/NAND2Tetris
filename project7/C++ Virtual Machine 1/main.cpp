#include <iostream>
#include <string>

/*
FROM THE BOOK:
The main program should construct a Parser to parse the VM input file and a
CodeWriter to generate code into the corresponding output file. It should then
march through the VM commands in the input file and generate assembly code for
each one of them.
*/

std::string returnWordFromCommandPhrase(int number)
{
    std::string command = "push constant 37";
    std::string leftover = command;
    std::string word;

    if (number < 1 || number > 3) return leftover;

    for (auto i = 0; i < number; i++) {
        leftover = leftover.substr(leftover.find_first_not_of(" "));
        word = leftover.substr(0, leftover.find(" "));
        leftover = leftover.substr(word.length());
    }
    return word;
}

int main()
{
    

    return 0;
}
