#include "SymbolTable.h"

void SymbolTable::addEntry(std::string symbol)
{
	std::string symbolAdress = std::string(16 - symbol.length(), '0') + old_string;
	std::string new_string = std::string(n_zero - old_string.length(), '0') + old_string;
	table.insert({ symbol, nextAddress });
	nextAddress++;
}

void SymbolTable::addEntry(std::string symbol, std::string address)
{
	std::string symbolAddress
	table.insert({ symbol, std::bitset<8>(address).to_string() });
}

bool SymbolTable::contains(std::string symbol)
{
	return (table.count(symbol)) ? true : false;
}

std::string SymbolTable::getAddress(std::string symbol)
{
	return table[symbol];
}

