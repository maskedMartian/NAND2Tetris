#include "Output.h"

Output::Output(std::string filename)
{
    std::string hackFilename = filename.substr(0, filename.find('.')) + ".hack";
    binFile.open(hackFilename);
}

void Output::writeToBin(std::string instruction)
{
    binFile << instruction << "\n";
}

void Output::closeFile()
{
    binFile.close();
}
