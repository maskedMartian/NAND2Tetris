// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

  @2   
  M=0     // R2 = 0

  @0
  D=M     // D = R0

  @END
  D; JEQ  // if R0 = 0 goto END

(LOOP)

  @1
  D=M     // move the value at R1 into the D register

  @2
  M=M+D   // add the value at R1 to the product held in R2

  @0
  M=M-1   // use R0 as a decrementing counter
  D=M     // move the counter value into D

  @LOOP
  D; JGT  // if D > 0 goto LOOP


(END)     // signal the end of the program with an infinite loop

  @END
  0; JMP
