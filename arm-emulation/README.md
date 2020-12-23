# project04-mchen81

ARM Emulation in C: ARMemu

Due Monday, March 23rd, at 11:59pm.


Please put all the deliverables into your CS 631 Project04 GitHub repository.

For this project you are going to write a C program that can execute ARM machine code by emulating the register state of an ARM CPU and emulating the execution of ARM instructions. We will call our emulator ARMemu (armemu).

You will not need to emulate every ARM instruction, just enough to execute the assembly programs you developed in Project03. You may have to modify/simplify some of your Project03 solutions to make them easier to emulate.

In class, I have showed you how to link assembled ARM code with compiled C code. You emulator will only need to execute assembly functions that are linked with the emulator itself.

Here are the basic ideas you need to implement:
A representation of the register state (r0-r15, CPSR)
A representation of memory (stack)
Given a function pointer and zero or more arguments, the ability to emulate the execution of the function
This could be an assembly program, or in principle a compiled C program.
The ability retrieve the return value from the emulated function
Dynamic analysis of the function execution:
Number of instructions executed
Conditional instructions such as beq and blt will count as one instruction regardless of the condition.
Instruction counts and percentages
Computation (data processing)
Memory
Branches
Number of branches taken
Number of branches not taken
Come up with a nice way to present a summary of the metrics
Cache simulation of ARM machine code execution
You will add a processor cache simulator to armemu to learn how caches work.
You can assume the block size in one word (4 bytes)
You should be able to support the following cache sizes in blocks (8, 32, 128, 1024)
You should simulate the following types of caches:
Direct mapped
Fully associative
Set associative (4 way)
For full credit you only need to emulate the instruction cache (not the data cache).
For a given cache size and cache type, for each emulated ARM function you should report:
Number of memory references
Cache misses
Cache hits
Hit ratio
Miss ratio
Note that the only memory you need to emulate is stack memory. You can assume all other memory for code and data resides in the address space of the process.

I will provide starter code.

You will want to become familiar with the ARM instruction set reference (PDF)


Deliverables
The ARMemu code
Demonstration that you can execute the Project03 assembly functions.
Including the recursive fib function
Include tests from Project03, show inputs and outputs
For each test function you want to show
The results of executing the C code
The results of executing the native ARM code
The results of emulating the ARM code
The dynamic analysis summary
The cache simulation summary

Instructions you need to emulate (not complete)
mov (reg or immediate)
add (reg or immediate)
sub (reg or immediate)
cmp (reg or immediate)
b, beq, bnq (others as needed)
bl, bx
ldr, str, ldrb
optional: push, pop
Others depending on what you used in your Project03 code

