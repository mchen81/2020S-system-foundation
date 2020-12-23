#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"arm.h"

int quadratic_c(int x, int a, int b, int c);
int quadratic_s(int x, int a, int b, int c);

void test_c(int x, int a, int b, int c){
    int r;
    r = quadratic_c(x, a, b, c);
    printf("quadratic_c(%d, %d, %d, %d) = %d \n", x, a, b, c, r);
}

void test_s(int x, int a, int b, int c){
    int r;
    r = quadratic_s(x, a, b, c);
    printf("quadratic_s(%d, %d, %d, %d) = %d \n", x, a, b, c, r);
}

void test_e(int x, int a, int b, int c, int single_step){
    struct arm_state state;
    arm_state_init(&state, (unsigned int * )quadratic_s, x, a, b, c);
    unsigned int r = armemu(&state, single_step);
    printf("quadratic_e(%d, %d, %d, %d) = %d \n", x, a, b, c, r);
    arm_metrics_print(&state);
    cache_state_stats(&state.cstate);
    arm_state_free(&state);
}

void single_step_mode(int argc, char *argv[]){
    if(strcmp(argv[1], "-s") != 0 || argc != 6){
        printf("To enter single step mode, input -s with parameters(x, a, b, c)\n");
        return;
    }
    int x, a, b, c;
    x = atoi(argv[2]);
    a = atoi(argv[3]);
    b = atoi(argv[4]);
    c = atoi(argv[5]);
    test_e(x, a, b, c, 1);
}

int main(int argc, char *argv[]){
    
    if(argc > 1){
        single_step_mode(argc, argv);
        return 0;
    }

    int x = 2, a = 3, b = 4, c = 1;
    test_c(x, a, b, c);
    test_s(x, a, b, c);
    test_e(x, a, b, c, 0);

    x = -3;
    a = -1;
    b = -8;
    c = -1;
    test_c(x, a, b, c);
    test_s(x, a, b, c);
    test_e(x, a, b, c, 0);

    x = 0;
    a = -3;
    b = 77;
    c = 8;
    test_c(x, a, b, c);
    test_s(x, a, b, c);
    test_e(x, a, b, c, 0);

    x = 3;
    a = 2;
    b = -2;
    c = -4;
    test_c(x, a, b, c);
    test_s(x, a, b, c);
    test_e(x, a, b, c, 0);

    return 0;
}
