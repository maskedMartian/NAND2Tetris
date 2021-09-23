#include "SymbolTable.h"

#include <bitset>

void SymbolTable::addEntry(std::string symbol)
{
    table.insert({ symbol, std::bitset<16>(nextAddress).to_string() });
    nextAddress++;
}

void SymbolTable::addEntry(std::string symbol, std::string address)
{
    table.insert({ symbol, address });
}

bool SymbolTable::contains(std::string symbol) const
{
    return (table.count(symbol)) ? true : false;
}

std::string SymbolTable::getAddress(std::string symbol)
{
    return table[symbol];
}
