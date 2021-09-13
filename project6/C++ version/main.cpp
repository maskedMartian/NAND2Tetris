#include <iostream>

#include "Code.h"
#include "Output.h"
#include "Parser.h"
#include "SymbolTable.h"

int main(int argc, char** argv) 
{
	Output output("foo.hack");
	Parser parser("foo.txt");
	
	while (parser.hasMoreCommands())
	{
	    parser.advance();
	    output.writeToBin(parser.command);
	}
	output.closeFile();
	parser.closeFile();
	
	SymbolTable symbolTable;
	
	std::cout << symbolTable.contains("R16") << "\n";
	std::cout << symbolTable.getAddress("THIS") << "\n";
	
	return 0;
}

