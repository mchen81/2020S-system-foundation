.global strlen_s
.func strlen_s

// r0 = *s
strlen_s:
    mov r1, #0  // int i = 0
strlen_loop:
    
    ldrb r2, [r0, r1]       // r2 = char of s[i]    
    cmp r2, #0             // if s[i] = \0
    beq strlen_end

    add r1, r1, #1      // i++
    b strlen_loop
strlen_end :
    mov r0, r1
    bx lr
.endfunc
