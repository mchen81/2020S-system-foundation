# 2020S-system-foundation
This repo has the programs that I learned and wrote for system foundation in 2020 Spring at USF. 

## Number converter
To convert a number(could be 2, 10, 16 based) into decimal(both signed and unsigned), 2-based and 16-based

## Arm assembly
This projects was to help me be famaliar with assembly and c. The assignments are 7 simple .c and .s programs.
1. quadratic (equation): given a, b, c, and x, count the result of a * (x * x) + b * x + c
2. sum_array: to sum up integer elements in an array
3. find_max: to find the max number in an integer array
4. sort: to sort an array in descending
5. fib_iter: Implement the fibonacci sequence function **iteratively**. Given a value n, find the nth fibonacci sequence value
6. fib_rec: Implement the fibonacci sequence function **recursively**. Given a value n, find the nth fibonacci sequence value.
7. strlen: a function that will determine dfthe length of a C string

## Arm emulation
This is an Arm emulator, which is able to operate processes in arm-assembly lists. 

## Tools fo xv6
The [xv6](https://pdos.csail.mit.edu/6.828/2012/xv6.html) is a simple operation system supporting basic Linux commands. Based on this os, 
we implemented system call tracer, listing running process status, and process hibernation functionalities, see the following commands. 
1. strace [a command]
2. ps
3. suspend [pid] [processname.proc]
4. resume [processname.proc]
