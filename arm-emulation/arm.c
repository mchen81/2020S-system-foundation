#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#include"recognize_inst.h"
#include"arm.h"
#include"cache.h"
#include"utils.h"

void arm_state_init(struct arm_state *as, unsigned int *func,
                    unsigned int arg0, unsigned int arg1,
                    unsigned int arg2, unsigned int arg3){

    int i;

    /* Zero out all registers */
    for (i = 0; i < NREGS; i++) {
        as->regs[i] = 0;
    }

    /* Zero out CPSR */
    as->cpsr = 0;

    /* Zero out the stack */
    for (i = 0; i < STACK_SIZE; i++) {
        as->stack[i] = 0;
    }

    /* Set the PC to point to the address of the function to emulate */
    as->regs[PC] = (unsigned int) func;

    /* Set the SP to the top of the stack (the stack grows down) */
    as->regs[SP] = (unsigned int) &as->stack[STACK_SIZE];

    /* Initialize LR to 0, this will be used to determine when the function has called bx lr */
    as->regs[LR] = 0;

    /* Initialize the first 4 arguments */
    as->regs[0] = arg0;
    as->regs[1] = arg1;
    as->regs[2] = arg2;
    as->regs[3] = arg3;

    /* Initialize metrics*/
    as->metrics.ic = 0;
    as->metrics.dpc = 0;
    as->metrics.mc = 0;
    as->metrics.bc = 0;
    as->metrics.bt = 0;
    as->metrics.bnt = 0;

    /* Initialize cache state*/
    cache_state_init(&as->cstate);

}

void arm_state_free(struct arm_state *as){
    cache_state_free(&as->cstate);
}

void arm_state_print(struct arm_state *as){
    printf("*************************************Arm State****************************************\n");
    int i;
    for (i = 0; i < 4; i++) {
        printf("reg[%02d] = %10u", i, as->regs[i]);
        if(i< 3){
            printf(", ");
        }
    }
    printf("\n");

    printf("reg[12] = %10u, ", as->regs[12]);
    printf("reg[SP] = 0x%08X, ", as->regs[SP]);
    printf("reg[LR] = 0x%08X, ", as->regs[LR]);
    printf("reg[PC] = 0x%08X\n", as->regs[PC]);

    printf("cpsr = %X\n", as->cpsr);
}

void arm_metrics_print(struct arm_state *as){
    // metrics info
    float ic = (float)as->metrics.ic;
    
    if(ic == 0.0){
        ic = 1.0; // avoid nan
    }

    printf("******************************Metrics*********************************\n");
    printf("Instruction count: %d\n", as->metrics.ic);
    printf(" Computation: %3d, Percentage: %.2f%\n", as->metrics.dpc, as->metrics.dpc/ic * 100.0);
    printf("      Memory: %3d, Percentage: %.2f%\n", as->metrics.mc, as->metrics.mc/ic * 100.0);
    printf("    Branches: %3d, Percentage: %.2f%\n", as->metrics.bc, as->metrics.bc/ic * 100.0);
    printf("Branch taken: %3d,  not taken: %d\n", as->metrics.bt, as->metrics.bnt);
}

void arm_update_cpsr(unsigned int op1, unsigned int op2, struct arm_state *state){ // NZCV
    int op1s = (int) op1;
    int op2s = (int) op2;
    int r = op1s - op2s;
    unsigned int cpsr = state->cpsr;
    cpsr = clear_bit(cpsr, CPSR_N);
    cpsr = clear_bit(cpsr, CPSR_Z);
    cpsr = clear_bit(cpsr, CPSR_C);
    cpsr = clear_bit(cpsr, CPSR_V);

    if(r < 0){
        cpsr = set_bit(cpsr, CPSR_N);
    }

    if(r == 0){
        cpsr = set_bit(cpsr, CPSR_Z);
    }

    if(op2s > op1s){
        cpsr = set_bit(cpsr, CPSR_C);
    }

    if(op1s > 0 && op2s < 0 && r < 0){
        cpsr = set_bit(cpsr, CPSR_V);
    }else if(op1s < 0 && op2s > 0 && r > 0){
        cpsr = set_bit(cpsr, CPSR_V);
    }

    state->cpsr = cpsr;
}

void armemu_dp(struct arm_state *state, unsigned int iw, int print){
    unsigned int rd, rn, rm, i, imm, opcode;
    char dp_inst[20];
    rd = (iw >> 12) & 0xF;
    rn = (iw >> 16) & 0xF;
    rm = iw & 0xF;
    i = (iw >> 25) & 0b1;
    imm = iw & 0xFF;
    opcode = (iw >> 21) & 0b1111;

    if(opcode == 0b0100){ // add
        state->regs[rd] = state->regs[rn] + (i == 0 ? state->regs[rm] : imm);
        
        if(i == 0){
            sprintf(dp_inst, "add r%d, r%d, r%d", rd, rn, rm);
        }else{
            sprintf(dp_inst, "add r%d, r%d, #%d", rd, rm, imm);
        }

    }else if(opcode == 0b0010){ //sub
        state->regs[rd] = state->regs[rn] - (i == 0 ? state->regs[rm] : imm);
        if(i == 0){
            sprintf(dp_inst, "sub r%d, r%d, r%d", rd, rn, rm);
        }else{
            sprintf(dp_inst, "sub r%d, r%d, #%d", rd, rm, imm);
        }
    }else if(opcode == 0b1101){ // mov
        state->regs[rd] = (i == 0 ? state->regs[rm] : imm);
        if(i == 0){
            sprintf(dp_inst, "mov r%d, r%d", rd, rm);
        }else{
            sprintf(dp_inst, "mov r%d, #%d", rd, imm);
        }    
    }else if(opcode == 0b1010){ //cmp
        // rn - rm
        unsigned int op1 = state->regs[rn];
        unsigned int op2 = (i == 0 ? state->regs[rm] : imm);
        arm_update_cpsr(op1, op2, state);
        if(i == 0){
            sprintf(dp_inst, "cmp r%d, r%d", rd, rm);
        }else{
            sprintf(dp_inst, "cmp r%d, #%d", rd, imm);
        }   
    }

    if(rd != PC){
        state->regs[PC] = state->regs[PC] + 4;
    }

    if(print){
        printf("iw: %x, inst: %s\n", iw, dp_inst);
    }
}

void armemu_bx(struct arm_state *state, unsigned int iw, int print){
    unsigned int rn;
    rn = iw & 0b1111;
    state->regs[PC] = state->regs[rn];
    
    if(print){
        printf("iw: %x, inst: bx lr\n", iw);
    }
}

void armemu_mul(struct arm_state *state, unsigned int iw, int print){
    unsigned int rd, rm, rs;
    rd = (iw >> 16) & 0xF;
    rm = iw & 0xF;
    rs = (iw >> 8) & 0xF;
    state->regs[rd] = state->regs[rm] * state->regs[rs];

    if(rd != PC){
        state->regs[PC] = state->regs[PC] + 4;
    }

    if(print){
        printf("iw: %x, inst: mul r%d, r%d, r%d\n", iw, rd, rm, rs);
    }
}

bool cpsr_check_cond(unsigned int cpsr, unsigned int cond){
    int Z = get_bit(cpsr, CPSR_Z);
    int N = get_bit(cpsr, CPSR_N);
    int V = get_bit(cpsr, CPSR_V);

    if(cond == 0b1110){ //ignore
        return true;
    }else if(cond == 0b0000 && Z == 1){ //eq
        return true;
    }else if(cond == 0b0001 && Z == 0){ //ne
        return true;
    }else if(cond == 0b1010 && N == V){ //ge
        return true;
    }

    return false;
}

void armemu_b(struct arm_state *state, unsigned int iw, int print){
    unsigned int cond, link, offset;
    bool execute;
    cond = (iw >> 28) & 0xF;
    execute = cpsr_check_cond(state->cpsr, cond);
    if(execute == false){
        state->regs[PC] = state->regs[PC] + 4;
        state->metrics.bnt = state->metrics.bnt + 1;
    }else{
        link = get_bit(iw, 24);
        offset = iw & 0xFFFFFF;

        unsigned int s = get_bit(offset, 23);
        if(s == 1){ //negative offset
            for(int i = 24; i < 32; i++){
                offset = set_bit(offset, i);
            }
        }

        offset = offset * 4;
        offset = offset + 8;
        if(link){
            state->regs[LR] =state->regs[PC] + 4;
        }
        state->regs[PC] = state->regs[PC] + offset;

        state->metrics.bt = state->metrics.bt + 1;
    }
    if(print){
        printf("iw: %x, b %s\n", iw, execute ? "taken" : "not taken");
    }
}

void armemu_sdt(struct arm_state *state, unsigned int iw, int print){
    unsigned int l, i, u, b, rn, rd, rm, imm;
    int offset;
    rd = (iw >> 12) & 0xF;
    rn = (iw >> 16) & 0xF;
    rm = iw & 0xF;
    l = (iw >> 20) & 0b1;
    i = (iw >> 25) & 0b1;
    u = (iw >> 23) & 0b1;
    b = (iw >> 22) & 0b1;
    imm = iw & 0xFF;

    offset = i == 0 ? (int)imm : (int)state->regs[rm];
    offset = u == 1 ? offset : -offset;

    unsigned int *addr = (unsigned int *)(state->regs[rn] + offset);
    unsigned char *addrb = (unsigned char *)(state->regs[rn] + offset);
    
    char inst[20];

    if(l == 1){
        // load
        state->regs[rd] = (b == 0 ? *addr : *addrb);
        sprintf(inst, "ldr%c r%d, [r%d, %d]\n", b==0 ? ' ' :'b', rd, rn, offset);
    }else{
        // store
        if(b == 0){
            *addr = state->regs[rd];
            sprintf(inst, "str  r%d, [r%d, %d]\n", rd, rn, offset);
        }else{
            *addrb = state->regs[rd];
            sprintf(inst, "strb r%d, [r%d, %d]\n", rd, rn, offset);
        }
    }

    if(rd != PC){
        state->regs[PC] = state->regs[PC] + 4;
    }

    if(print){
        printf("iw: %x, inst: %s",iw, inst);
    }
}

void armemu_one(struct arm_state *state, int print){
    unsigned int iw;

    iw = cache_lookup(&state->cstate, state->regs[PC]);
    state->metrics.ic = state->metrics.ic + 1;

    if (is_bx_inst(iw)) {
        armemu_bx(state, iw, print);
        state->metrics.bc = state->metrics.bc + 1;
        state->metrics.bt = state->metrics.bt + 1;
    } else if (is_mul_inst(iw)){
        armemu_mul(state, iw, print);
        state->metrics.dpc = state->metrics.dpc + 1;
    } else if (is_dp_inst(iw)) {
        armemu_dp(state, iw, print);
        state->metrics.dpc = state->metrics.dpc + 1;
    } else if (is_sdt_inst(iw)){
        armemu_sdt(state, iw, print);
        state->metrics.mc = state->metrics.mc + 1;
    } else if (is_b_inst(iw)){ 
        armemu_b(state, iw, print);
        state->metrics.bc = state->metrics.bc + 1;
    }
}

unsigned int armemu(struct arm_state *state, int single_step){
    if(single_step){
        printf("=========Single Step Mode========\n");
        printf("Please input the following keys:\n");
        printf("n -> execute the next instruction\n");
        printf("c -> print current cache states\n");
        printf("m -> print current metrics info\n");
        printf("a -> print current regs state\n");
        printf("r -> finish and print the result\n");
        printf("=================================\n");
        bool end = false;
        char arg[1];
        while(state->regs[PC] != 0){
            if(end){
                armemu_one(state, 0);
                continue;
            }

            scanf("%s", arg);
            if(arg[0] == 'n'){
                armemu_one(state, 1);
            }else if(arg[0] == 'c'){
                cache_state_stats(&(state->cstate));
            }else if(arg[0] == 'm'){
                arm_metrics_print(state);
            }else if(arg[0] == 'a'){
                arm_state_print(state);
            }else if(arg[0] == 'r'){
                end = true;
            }else{
                printf("Wrong argument, only support n, c, m, a, and r\n");
                continue;
            }
        }
    }else{
        while (state->regs[PC] != 0) {
            armemu_one(state, 0);
        }
    }
    return state->regs[0];
}
