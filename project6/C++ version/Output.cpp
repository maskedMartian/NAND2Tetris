#include "Output.h"

Output::Output(std::string fileName) : binFile{ fileName }
{
}

void Output::writeToBin(std::string instruction)
{
    binFile << instruction << "\n";	
}
    
void Output::closeFile()
{
	binFile.close();
}

