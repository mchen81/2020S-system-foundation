.global find_max_s
.func find_max_s
/*
    r0  = array
    r1  = leni, #4
    r2  = i
    r3  = max
*/

find_max_s:

    sub sp, sp, #16
    mov r2, #1      // i = r2 = 1
    ldr r3, [r0]    // max = r3 = array[0]

    str r1, [sp, #4]    // len
    str r2, [sp, #8]    // i

find_max_loop_start:

    ldr r1, [sp, #4] // len
    ldr r2, [sp, #8] // i
    cmp r2, r1       // if i >= len -> end
    bge find_max_loop_end

    mov r1, #4
    mul r2, r1, r2
    ldr r1, [r0, r2]
    cmp r3, r1
    bge find_max_loop_continue
    mov r3, r1

find_max_loop_continue: // i++
    ldr r2, [sp, #8]
    add r2, r2, #1
    str r2, [sp, #8]
    b find_max_loop_start

find_max_loop_end:
    add sp, sp, #16
    mov r0, r3
    bx lr

.endfunc
