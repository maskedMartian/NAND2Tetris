#include <iostream>
#include <string>

/*
FROM THE BOOK:
The main program should construct a Parser to parse the VM input file and a
CodeWriter to generate code into the corresponding output file. It should then
march through the VM commands in the input file and generate assembly code for
each one of them.
*/

int main()
{
    std::cout << "Hello world!";
    std::string command = "[    Hey Dude! - // 700 $ and foo + spam   ]";
    std::cout << "\n\nOld String : " << command << "\n";
    command.erase(std::remove_if(command.begin(), command.end(), isspace), command.end());
    std::cout << "\n\nNew String : " << command << "\n";

    return 0;
}
