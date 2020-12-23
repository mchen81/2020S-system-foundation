#include<stdbool.h>
#include"recognize_inst.h"

bool is_dp_inst(unsigned int iw){
    unsigned int op;
    op = (iw >> 26) &0b11;
    return op == 0;
}

bool is_mul_inst(unsigned int iw){
    unsigned int op, operand;
    op = (iw >> 22) & 0b111111;
    operand = (iw >> 4) & 0b1111;
    return (op == 0) && (operand == 0b1001);
}

bool is_bx_inst(unsigned int iw){
    unsigned int bx_code;
    bx_code = (iw >> 4) & 0x00FFFFFF;
    return (bx_code == 0b000100101111111111110001);
}

bool is_b_inst(unsigned int iw){
    unsigned int b_code;
    b_code = (iw >> 25) & 0b111;
    return b_code == 0b101;
}

bool is_sdt_inst(unsigned int iw){
    unsigned int op;
    op = (iw >> 26) & 0b11;
    return op == 1;
}
