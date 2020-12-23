.global quadratic_s
.func quadratic_s

/*
    r0 = x 
    r1 = a
    r2 = b
    r3 = c
*/

quadratic_s:
    
    mul r12, r0, r0     // r12 = x * x
    mul r1, r12, r1     // a = x^2 * a
    mul r2, r0, r2      // b = x * b
    add r0, r1, r2      // r0 = ax2 + bx
    add r0, r0, r3      // r0 = ax2 + bx + c
    bx lr

.endfunc
