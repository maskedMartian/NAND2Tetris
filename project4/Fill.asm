// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

  @0
  M=0       // initialize R0 to 0

(TOP)
  
  @KBD
  D=M       // load the value at the base address of KBD into D

  @BLACK
  D;JNE     // jump to BLACK if KBD !=0 (a key is being pressed)

  @CLEAR
  D;JEQ     // jump to CLEAR if KBD == 0 (no key is being pressed)

  @TOP
  0;JMP     // unconditional jump to TOP 


//----------------------------------------------------------------------------
(BLACK)     // subroutine to blacken the screen

  @0
  D=M       // load R0 into D

  @8192     // load the highest offset value of SCREEN memory
  D=D-A     // subtract the constant 8192 from the value in R0

  @TOP
  D;JEQ     // jump to TOP if D == 0 (R0 == 8192 - the screen is full black)

  @0
  D=M       // load R0 into D

  @SCREEN   // load the base address of SCREEN into A
  A=A+D     // add the offset stored in R0

  M=-1      // blacken the 16 pixels referenced by address A 
            // by setting each bit to 1 (-1 = 1111 1111 11111 111)
  @0
  M=M+1     // increment the offset stored in R0

  @TOP
  0;JMP     // unconditional jump to TOP


//----------------------------------------------------------------------------
(CLEAR)     // subroutine to clear the screen

  @0
  D=M       // load R0 into D

  @TOP
  D;JLT     // jump to TOP if R0 < 0 (all pixels of SCREEN have been cleared)

  @SCREEN   // load the base address of SCREEN into A
  A=A+D     // add the offset stored in R0

  M=0      // blacken the 16 pixels referenced by address A 

  @0
  M=M-1     // increment the offset stored in R0

  @TOP
  0;JMP     // unconditional jump to TOP