class Output:

	# opens an output file where the assembled machine code will be stored  
    def __init__(self, sourceFile):
        # copy source file name and change .asm file extension to .hack
        outputFile = sourceFile[0:-3] + "hack"
        self._binFile = open(outputFile, "wt")

    # writes a single machine language instruction to the output file 
    def writeToBin(self, instruction : str):
    	self._binFile.write(instruction + "\n")

    # close the output file
    def closeFile(self):
    	self._binFile.close()