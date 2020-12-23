.global fib_rec_s
.func fib_rec_s

fib_rec_s:
    // r0  
    
    cmp r0, #0
    beq return0
    cmp r0, #1
    beq return1
    
    // stack
    sub sp, sp, #16
    str lr, [sp]
    
    // store current n at 4
    str r0, [sp, #4]

    // n-1
    sub r0, r0, #1
    bl fib_rec_s
    str r0, [sp, #8] // store result
    // n-2
    ldr r0, [sp, #4] // load r0
    sub r0, r0, #2 // n-2
    bl fib_rec_s
    
    ldr r1, [sp, #8]
    add r0, r0, r1
    b end

return0:
    mov r0, #0
    bx lr
return1:
    mov r0, #1
    bx lr
end:
    ldr lr, [sp]
    add sp, sp, #16
    bx lr
.endfunc
