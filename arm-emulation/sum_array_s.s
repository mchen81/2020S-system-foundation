.global sum_array_s
.func sum_array_s

/*
    r0 = *array
    r1 = len(n)
*/

sum_array_s:

    mov r2, #0      // r2 = i = 0
    mov r3, #0      // r3 = sum = 0

sum_array_loop_start:

    cmp r2, r1      // if(i >= leng) break
    beq sum_array_loop_end
    ldr r12, [r0]   // r12 = array[i]
    add r3, r3, r12 // sum  =  sum  + array[i]
    add r0, r0, #4  // array[i++]  
    add r2, r2, #1  // i++
    b sum_array_loop_start

sum_array_loop_end:
    
    mov r0, r3
    bx lr

.endfunc





