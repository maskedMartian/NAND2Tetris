import sys
from myCode import Code
from myParser import Parser
from mySymbolTable import SymbolTable
from myOutput import Output


C_COMMAND_PREFIX = "111"
PARAMETER_LIST   = sys.argv
SOURCE_FILE_NAME = sys.argv[1]


# checks whether the name of a source file was entered at the command line as an 
# argument to the program
def noCommandLineArguements():
    return True if len(PARAMETER_LIST) < 2 else False

# assembles a hack assembly language source file into hack machine language
def main():
    if noCommandLineArguements():
        print("ERROR: No source file specified")
        exit()

    parser = Parser(SOURCE_FILE_NAME)
    output = Output(SOURCE_FILE_NAME)
    symbolTable = SymbolTable()
    code = Code()

    # first pass
    while parser.hasMoreCommands():
        parser.advance() # read next command from source file
        if parser.commandType() == "L_COMMAND": # label
            if not symbolTable.contains(parser.symbol()):   
                symbolTable.addEntry(parser.symbol(), parser.getAddress())
    parser.resetFile() # reset file pointer to first line of source file

    # second pass
    while parser.hasMoreCommands():
        parser.advance() # read next command from source file
        if parser.commandType() == "A_COMMAND": # addressing command
            if parser.isConstant():
                output.writeToBin(parser.getConstant())
            else:
                if not symbolTable.contains(parser.symbol()):
                    symbolTable.addEntry(parser.symbol(), "new")
                output.writeToBin(symbolTable.getAddress(parser.symbol()))
        elif parser.commandType() == "C_COMMAND": # computation command
            output.writeToBin(C_COMMAND_PREFIX + code.comp(parser.comp())
                + code.dest(parser.dest()) + code.jump(parser.jump()))
        elif parser.commandType() != "L_COMMAND": # label
            print("ERROR: Unexpected command")
            exit()

    parser.closeFile()
    output.closeFile()


# Program Entry
main()