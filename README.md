# NAND2Tetris

Programming assignments from: 
The Elements of Computing Systems: 
Building a Modern Computer from First Principles 
as part of the online Coursera course.

The first five projects were completed in an HDL.

I originally wrote the sixth project in Python as I was focused on learning it 
at the time I took part one of the online course. On 10/19/2021 I completed the
project in C++. I am currently focused on improving my knowledge and experience
with modern C++ as that is the primary language I will be using in my new
position I just started four months ago.

I'm considering completing all the projects in other languages upon completion 
of part two of the course. Languages I'm considering are JavaScript, Python, 
and Rust. JavaScript is becoming more of a favorite as I practice using it more
and more and continue to learn React. Rust is especially interesting to me as I
started learning it a few years a go, but had to put in on the shelf to
contrate on other things. Once I feel more confident using modern C++, I want
to continue learning and practicing using Rust. I hope we eventually do a
project at work using Rust. 

Project 6: HACK assembler
* HACK is a small, simple assembly language that runs on the HACK processor
  architecture designed for this course.
* HACK is a Turing Complete architecture/language that is capable of performing
  any algorithm any modern computer can perform
* HACK consists of mathematical operations (addition and subtraction), logical
  operations (and, or, not), memory access operations, and conditional branching
  instructions (jumps).
* HACK jump instructions:
    - JEQ - jump if equal to zero
    - JNE - jump if not equal to zero
    - JGT - jump if greater than zero
    - JLT - jump if less than zero
    - JGE - jump if greater than or equal to zero
    - JLE - jump if less than or equalt to zero
    - JMP - jump (unconditional)
* HACK allows the use of labels and variable names for memory locations
* HACK uses all alphanumeric characters as well as the special characters:<br>
  @ ( ) ; the math symbols + - = and the logic symbols & | !
* HACK uses the character combination // for single line comments
* HACK allows the use of two registers in the CPU, D and A
* The D register can store the results of an operation performed in the
  processor or values copied to it from the A register
* The A register can store the results of an operation performed in the
  processor or values copied to it from the D register
* The A register can also be accessed as the M register - Using A references
  the value stored in the register, using M references the value stored at M[A]
  where M is memory (RAM) and the value held in A is the memory address
* The A register can be loaded with constants or variables - the use of the @
  symbol followed by a value loads that value into the A register
  Examples:<br>
      @10  // loads the value 10 into the A register<br>
      @myVariable  // loads the contents of myVariable into the A register
* labels for jump instructions are surrounded by ( and )
* HACK has 23 predefined symbols in its symbol table (each is followed by its
  memory address):
    - SP         0
    - LCL        1
    - ARG        2
    - THIS       3
    - THAT       4
    - R0 - R15   0 - 15
    - SCREEN     16384
    - KBD        24576
* User defined symbols (labels and variable names) are copied to the symbol
  table starting with address 16 and continuing from there
* A HACK assembly program terminates when it detects an infinite loop
* An example of a simple HACK assembly program:<br>
    // Adds 1 + 2 + ... + 100<br>
        @i<br>
        M=1     // i = 1<br>
        @sum<br>
        M=0     // sum = 0<br>
    (LOOP)<br>
        @i<br>
        D=M     // D = i<br>
        @100<br>
        D=D-A   // D = i - 100<br>
        @END<br>
        D;JGT   // if D > 0 goto END<br>
        @i<br>
        D=M     // D = i<br>
        @sum<br>
        M=D+M   // sum = sum + 1<br>
        @i<br>
        M=M+1   // i = i + 1<br>
        @LOOP<br>
        0;JMP   // goto loop<br>
    (END)<br>
        @END<br>
        0;JMP   // inifinite loop<br>
* In the example program above, the following symbols are added to the symbol
  table (followed by memory address):
    - i     16
    - sum   17
    - LOOP  18
    - END   19