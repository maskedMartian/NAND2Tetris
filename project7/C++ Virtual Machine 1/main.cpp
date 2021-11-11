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
    std::cout << "Hello world!\n\n";

    std::cout << "======================================\n";
    std::cout << "Var0 = ---" << returnWordFromCommandPhrase(0) << "---\n";
    std::cout << "Var1 = ---" << returnWordFromCommandPhrase(1) << "---\n";
    std::cout << "Var2 = ---" << returnWordFromCommandPhrase(2) << "---\n";
    std::cout << "Var3 = ---" << returnWordFromCommandPhrase(3) << "---\n";
    std::cout << "Var4 = ---" << returnWordFromCommandPhrase(4) << "---\n";

    return 0;
}
