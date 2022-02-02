#pragma once

enum class CommandTypes
{
    c_arithmetic,
    c_push,
    c_pop,
    c_label,
    c_goto,
    c_if,
    c_function,
    c_return,
    c_call,
    c_none
};

#define C_ARITHMETIC CommandTypes::c_arithmetic
#define C_PUSH       CommandTypes::c_push
#define C_POP        CommandTypes::c_pop
#define C_LABEL      CommandTypes::c_label
#define C_GOTO       CommandTypes::c_goto
#define C_IF         CommandTypes::c_if
#define C_FUNCTION   CommandTypes::c_function
#define C_RETURN     CommandTypes::c_return
#define C_CALL       CommandTypes::c_call
#define C_NONE       CommandTypes::c_none
