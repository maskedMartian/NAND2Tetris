#ifndef OUTPUT_H
#define OUTPUT_H

#include <fstream>
#include <string>

class Output
{
public:
	Output(std::string fileName);
    void writeToBin(std::string instruction);
    void closeFile();
private:
	std::ofstream binFile;
};

#endif

