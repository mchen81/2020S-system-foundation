.global fib_iter_s
.func fib_iter_s

fib_iter_s:

    mov r1, #0  // r1 = pre_pre_num = 0
    mov r2, #0  // r2 = pre_num = 0
    mov r3, #1  // r3 = cur_num = 1
    
    cmp r0, #0  // if(n == 0) return 0;
    beq return_0
    
    mov r12, #1 // int i = 1

fib_iter_loop_start:
    
    cmp r12, r0 // if i >= n -> end
    bge fib_iter_loop_end
    
    mov r1, r2
    mov r2, r3
    add r3, r1, r2

fib_iter_loop_continue:
    add r12, r12, #1
    b fib_iter_loop_start

fib_iter_loop_end:
    mov r0, r3
    bx lr

return_0:
    mov r0, #0
    bx lr
.endfunc
