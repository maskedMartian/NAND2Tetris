class Parser:  

    # open the hack assembly language source file to be assembled and initialize 
    # _lineAddress to the first line of the program
    def __init__(self, inputFile : str):
        # check for correct file extension
        if inputFile[-4:] != ".asm":
            print("ERROR: Assembly file expected")
            exit()
        # open input and output files
        try:
            self._asmFile = open(inputFile, "rt")
        except FileNotFoundError:
            print("ERROR: Specified source file does notexist")
            exit()
        except:
            print("ERROR: Something went wrong trying to open the necessary files")
            exit()
        self._lineAddress = 0

   # checks to see if the source file contains more commands - checks for EOF
   # while ignoring lines that are whitespace or contain only comments
    def hasMoreCommands(self) -> bool:
        position = self._asmFile.tell()
        line = notEOF = self._asmFile.readline()
        # ignores lines that are whitespace or just comments with no commands
        while (notEOF and line.isspace() or line[:2] == "//" or 
               "//" in line and line[:line.index("//")].isspace()):
            position = self._asmFile.tell()
            line = notEOF = self._asmFile.readline()
        if notEOF: 
            # move file pointer back one line because last line read was a command
            self._asmFile.seek(position) 
            return True
        else:
            return False # EOF    

    # reads the next line from the source file into the parser and removes all
    # surrounding whitespace and any comments
    def advance(self): 
        self._command = self._asmFile.readline()
        if "//" in self._command:
            self._command = self._command[:self._command.index("//")]
        self._command = self._command.strip()
        if self.commandType() == "A_COMMAND" or self.commandType() == "C_COMMAND":
            self._lineAddress += 1

    # returns the type of command - all commands are one of three types
    def commandType(self) -> str:
        if self._command[0] == "@":
            return "A_COMMAND" # addressing command
        elif self._command[0] == "(" and self._command[-1] == ")":
            return "L_COMMAND" # label
        else:
            return "C_COMMAND" # computation command

    # returns the symbol (or constant) from an addressing command or the symbol
    # from a jump label
    def symbol(self) -> str:
        return (self._command[1:] 
            if self._command[0] == "@" else self._command[1:-1])
        
    # returns the destination mnemonic from a computation command
    def dest(self) -> str:
        return (self._command[:self._command.index("=")] 
            if "=" in self._command else "") 
    
    # returns the computation mnemonic from a computation command
    def comp(self) -> str:
        return (self._command[self._command.index("=") + 1:] 
            if "=" in self._command else self._command[:self._command.index(";")])
        
    # returns the jump mnemonic from a computation command
    def jump(self) -> str:
        return (self._command[self._command.index(";") + 1:] 
            if ";" in self._command else "") 
        
    # checks whether an addressing command references a constant
    def isConstant(self) -> bool:
        return True if self.symbol().isdecimal() else False

    # returns the address of a label
    def getAddress(self) -> str:
        if self.commandType() == "L_COMMAND":
            return str(bin(self._lineAddress))[2:].zfill(16)

    # returns the binary representation of a constant
    def getConstant(self) -> str:
        if self.commandType() == "A_COMMAND":
            return str(bin(int(self.symbol())))[2:].zfill(16)
        
    # set the file pointer of the source file back to the beginning of the file
    def resetFile(self):
        self._asmFile.seek(0)

    # close the source file
    def closeFile(self):
        self._asmFile.close()