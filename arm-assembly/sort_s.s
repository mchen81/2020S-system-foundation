.global sort_s
.func sort_s

sort_s:
    // lr = sp
    // r0 = array[0] => #4 
    // r1 = len      => #8
    // r2 = int i    => #12

    mov r2, #0  // int i = 0
    sub sp, sp, #16
    // store caller saved
    str lr, [sp]
    str r0, [sp, #4]  // &array[0]
    str r1, [sp, #8]  // len
    str r2, [sp, #12] // current i

sort_loop_start:
    ldr r0, [sp, #4]    // load &array[0]
    ldr r1, [sp, #8]    // load length
    ldr r2, [sp, #12]   // load i

    cmp r2, r1          // if i >= length : loop end
    bge sort_loop_end
    
    sub r1, r1, r2      // len = len -i (array[i]'s length = original length -i)
    mov r3, #4           // r3 = 4
    mul r2, r3, r2       // i = i * 4
    add r0, r0, r2       // r0 = &array[i]
    bl find_max_index_s  // r0 = index of max value from array[i]
    
    ldr r2, [sp, #12]   // get back i
    add r12, r0, r2     // r12 = max_index of array[i] + i (real max_index)
    cmp r12, r2         // if max_index = i, don't need to switch
    beq sort_loop_continue

    // switch array[i] <-> array[max_index]
    // load back &array[0] into r0
    ldr r0, [sp, #4] 
    
    mov r3, #4
    mul r2, r3, r2      // r2  = 4 * i 
    mul r12, r3, r12    // r12 = 4 * max_index

    ldr r1, [r0, r2]    // r1 = tmp = array[i];
    ldr r3, [r0, r12]   // r3 = array[max_index]
    str r3, [r0, r2]    // array[i] = r3  
    str r1, [r0, r12]   // array[max_index] = r1
    str r0, [sp, #4]    // replace original sorted array

sort_loop_continue:
    ldr r2, [sp, #12]    // load r12 as i
    add r2, r2, #1       // r2 = i++
    str r2, [sp, #12]    // str r2 to #12
    b sort_loop_start    // go back loop

sort_loop_end:
    ldr r0, [sp, #8]
    ldr lr, [sp]
    add sp, sp, #16
    bx lr

.endfunc

.func find_max_index_s
/*
    r0  = array
    r1  = len, #4
    r2  = i #8
    r3  = max value
    r12 = max index
*/

find_max_index_s:

    sub sp, sp, #16
    mov r2, #1      // i = r2 = 1
    ldr r3, [r0]    // max = r3 = array[0]
    mov r12, #0     // index = r12 = 0

    str lr, [sp]
    str r1, [sp, #4]    // len
    str r2, [sp, #8]    // i

find_max_index_loop_start:

    ldr r1, [sp, #4] // len
    ldr r2, [sp, #8] // i
    cmp r2, r1  // if i >= len -> end
    bge find_max_index_loop_end

    // r1, r2 avalible
    mov r1, #4
    mul r2, r1, r2
    ldr r1, [r0, r2]
    cmp r3, r1
    bge find_max_index_loop_continue
    mov r3, r1
    ldr r12, [sp, #8] // max index(r12) = current i

find_max_index_loop_continue: // i++
    ldr r2, [sp, #8]
    add r2, r2, #1
    str r2, [sp, #8]
    b find_max_index_loop_start

find_max_index_loop_end:
    ldr lr, [sp]
    add sp, sp, #16
    mov r0, r12
    bx lr

.endfunc
