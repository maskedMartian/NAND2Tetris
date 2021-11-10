#include <iostream>
#include <string>

/*
FROM THE BOOK:
The main program should construct a Parser to parse the VM input file and a
CodeWriter to generate code into the corresponding output file. It should then
march through the VM commands in the input file and generate assembly code for
each one of them.
*/

std::string returnWord(int number)
{

}

void fn(std::string command)
{
    std::string var1, var2, var3;
    var1 = ""; var2 = ""; var3 = "";

    std::cout << "\nCommand: " << command << "\n" << "=========================================================\n";
    std::cout << "Command = ---" << command << "---\n";

    // remove leading whitespace
    command = command.substr(command.find_first_not_of(" "));
    var1 = command.substr(0, command.find(" "));
    command = command.substr(var1.length());
    // remove leading whitespace
    command = command.substr(command.find_first_not_of(" "));
    var2 = command.substr(0, command.find(" "));
    command = command.substr(var2.length());
    // remove leading whitespace
    command = command.substr(command.find_first_not_of(" "));
    var3 = command.substr(0, command.find(" "));

    std::cout << "Command = ---" << command << "---\n";
    std::cout << "Var1 = ---" << var1 << "---\n";
    std::cout << "Var2 = ---" << var2 << "---\n";
    std::cout << "Var3 = ---" << var3 << "---\n";
}

int main()
{
    std::cout << "Hello world!\n\n";

    std::string command = "   push constant 37   ";
 
    fn(command);
    fn("push crazy 8");

    // remove trailing whitespace
    // command = command.substr(0, command.find_last_not_of(" ") + 1);



    /*
    * ltrim
    size_t start = command.find_first_not_of(" ");
    return (start == std::string::npos) ? "" : s.substr(start);
 
     rtrim
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
    */
    
    // var1 = command.substr(0, command.find(" "));

    // var2 = command.substr(command.find(" ")+1, command.find(" "));
    //var1 = command.substr(0, command.find(" "));

    //command = command.substr(0, command.find("//")); 

    // std::cout << "\n\nOld String : " << command << "\n";
    // command.erase(std::remove_if(command.begin(), command.end(), isspace), command.end());
    // std::cout << "\n\nNew : \n--" << var1 << "--\n--" << var2 << "\n";

    return 0;
}
