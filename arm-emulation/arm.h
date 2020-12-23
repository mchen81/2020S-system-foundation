#include<stdbool.h>
#include"cache.h"

#ifndef __arm
    #define __arm
    #define NREGS 16
    #define STACK_SIZE 1024
    #define SP 13
    #define LR 14
    #define PC 15

    // CPSR flags bit position
    #define CPSR_N 31
    #define CPSR_Z 30
    #define CPSR_C 29
    #define CPSR_V 28

    struct arm_state{
        unsigned int regs[NREGS];
        unsigned int cpsr;
        unsigned char stack[STACK_SIZE];
        struct cache_state cstate;

        /* metrics */
        struct inst_count{
            int ic;     // instruction count
            int dpc;    // data process count
            int mc;     // memory process
            int bc;     // brach process
            int bt;     // brach taken
            int bnt;    // branch not taken
        } metrics;
    };

    extern void arm_state_init(struct arm_state *as, unsigned int *func,
                               unsigned int arg0, unsigned int arg1,
                               unsigned int arg2, unsigned int arg3);
    extern void arm_state_print(struct arm_state *as);
    extern void arm_metrics_print(struct arm_state *as);

    // armemu functions
    extern void armemu_dp(struct arm_state *state, unsigned int iw, int print);
    extern void armemu_mul(struct arm_state *state, unsigned int iw, int print);
    extern void armemu_bx(struct arm_state *state, unsigned int iw, int print);
    extern void armemu_b(struct arm_state *state, unsigned int iw, int print);
    extern void armemu_sdt(struct arm_state *state, unsigned int iw, int print);
    // test helper functions
    extern void armemu_one(struct arm_state *state, int print);
    extern unsigned int armemu(struct arm_state *state, int single_step);

    extern void arm_state_free(struct arm_state *as);
    extern void arm_state_print(struct arm_state *as);

#endif
