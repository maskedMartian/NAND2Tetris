class Code:
    
    # returns the 3-bit code which corresponds to the given destination mnemonic 
    def dest(self, mnemonic : str) -> str:
        register = ["", "M", "D", "MD", "A", "AM", "AD", "AMD"]
        if mnemonic in register:
            return str(bin(register.index(mnemonic)))[2:].zfill(3)

    # returns the 7-bit code which corresponds to the given computation mnemonic 
    def comp(self, mnemonic : str) -> str:
        operation = {
              "0" : "0101010",   "1" : "0111111",  "-1" : "0111010", 
              "D" : "0001100",   "A" : "0110000",  "!D" : "0001101", 
             "!A" : "0110001",  "-D" : "0001111",  "-A" : "0110011", 
            "D+1" : "0011111", "A+1" : "0110111", "D-1" : "0001110", 
            "A-1" : "0110010", "D+A" : "0000010", "D-A" : "0010011", 
            "A-D" : "0000111", "D&A" : "0000000", "D|A" : "0010101",
              "M" : "1110000",  "!M" : "1110001", "M+1" : "1110111", 
            "M-1" : "1110010", "D+M" : "1000010", "D-M" : "1010011", 
            "M-D" : "1000111", "D&M" : "1000000", "D|M" : "1010101"
        }    
        return operation[mnemonic] if mnemonic in operation else "0000000"
        
    # returns the 3-bit code which corresponds to the given jump mnemonic 
    def jump(self, mnemonic : str) -> str:
        jumpType = ["", "JGT", "JEQ", "JGE", "JLT", "JNE", "JLE", "JMP"]
        if mnemonic in jumpType:
            return str(bin(jumpType.index(mnemonic)))[2:].zfill(3)