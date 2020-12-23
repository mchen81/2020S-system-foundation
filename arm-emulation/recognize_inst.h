#include<stdbool.h>
#ifndef __recognize_inst 
    #define __recognize_inst
    extern bool is_dp_inst(unsigned int iw);
    extern bool is_mul_inst(unsigned int iw);
    extern bool is_bx_inst(unsigned int iw);
    extern bool is_b_inst(unsigned int iw);
    extern bool is_sdt_inst(unsigned int iw);
#endif
