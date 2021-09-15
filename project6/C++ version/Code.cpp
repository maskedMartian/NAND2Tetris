#include "Code.h"

std::string Code::dest(std::string mnemonic)
{
    return machineRegister[mnemonic];
}

std::string Code::comp(std::string mnemonic)
{
    return operation[mnemonic];
}

std::string Code::jump(std::string mnemonic)
{
    return jumpType[mnemonic];
}

