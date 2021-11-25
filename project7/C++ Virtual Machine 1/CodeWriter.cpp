#include "CodeWriter.h"

// Opens the output file/stream and gets ready to write into it
CodeWriter::CodeWriter(/*output file stream*/)
{

}

// Informs the code writer that the translation of a new VM file is started
void CodeWriter::setFileName(std::string fileName)
{

}

// Writes the assembly code that is that is the translation of the given arithmetic command
void CodeWriter::writeArithmetic(std::string command)
{

}

// Writes the assembly code that is the translation of the given command, where command is C_PUSH
// or C_POP
void CodeWriter::WritePushPop(PushPopCommands command, std::string segment, int index)
{

}

// Closes the output file
void CodeWriter::close()
{

}
