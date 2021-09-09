class SymbolTable:

    # construct a symbol table which contains all the predefined symbols of the 
    # hack assembly langugage and initialize the next addresss in the table
    def __init__(self):
        self._table = {
            "SP"     : "0000000000000000",
            "LCL"    : "0000000000000001",
            "ARG"    : "0000000000000010",
            "THIS"   : "0000000000000011",
            "THAT"   : "0000000000000100",
            "R0"     : "0000000000000000",
            "R1"     : "0000000000000001",
            "R2"     : "0000000000000010",
            "R3"     : "0000000000000011",
            "R4"     : "0000000000000100",
            "R5"     : "0000000000000101",
            "R6"     : "0000000000000110",
            "R7"     : "0000000000000111",
            "R8"     : "0000000000001000",
            "R9"     : "0000000000001001",
            "R10"    : "0000000000001010",
            "R11"    : "0000000000001011",
            "R12"    : "0000000000001100",
            "R13"    : "0000000000001101",
            "R14"    : "0000000000001110",
            "R15"    : "0000000000001111",
            "SCREEN" : "0100000000000000",
            "KBD"    : "0110000000000000",
        }
        self._nextAddress = 16 

    # add a new entry into the symbol table
    def addEntry(self, symbol : str, address : str):
        if address == "new":
            address = str(bin(self._nextAddress))[2:].zfill(16)
            self._nextAddress += 1
        self._table.setdefault(symbol, address)

    # check whether the symbol table already contains a symbol
    def contains(self, symbol : str) -> bool:
        return True if symbol in self._table else False

    # return the corresponding memory address of a symbol from the symbol table 
    def getAddress(self, symbol : str) -> str:
        return self._table[symbol]
