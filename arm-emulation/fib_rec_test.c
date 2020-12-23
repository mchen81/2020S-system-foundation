#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"arm.h"

int fib_rec_c(int n);
int fib_rec_s(int n);

void test_c(int n){
    int c = fib_rec_c(n);
    printf("The fibonacci of %d found by C is %d\n", n ,c);
}

void test_s(int n){
    int s = fib_rec_s(n);
    printf("The fibonacci of %d found by S is %d\n", n ,s);
}

void test_e(int n, int single_step){
    struct arm_state state;
    arm_state_init(&state, (unsigned int * )fib_rec_s, n, 0, 0, 0);
    unsigned int r = armemu(&state, single_step);
    printf("The fibonacci of %d found by E is %d\n", n ,r);
    arm_metrics_print(&state);
    cache_state_stats(&state.cstate);
    arm_state_free(&state);
}

void single_step_mode(int argc, char *argv[]){
    if(strcmp(argv[1], "-s") != 0 || argc != 3){
        printf("To enter single step mode, input -s with one fibonacci index\n");
        return;
    }
    int f = atoi(argv[2]);
    test_e(f, 1);
}

int main(int argc, char *argv[]){
     if(argc > 1){
        single_step_mode(argc, argv);
        return 0;
    }

    for(int i = 0; i<= 20; i++){
        test_c(i);
        test_s(i);
        test_e(i, 0);
        printf("=================================================\n");
    }
    return 0;
}
