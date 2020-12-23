#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"arm.h"

int strlen_s(char *s);

void test_strlen(char *s, int single_step){
    struct arm_state state;
    arm_state_init(&state, (unsigned int * )strlen_s, (unsigned int)s, 0, 0, 0);
    unsigned int r = armemu(&state, single_step);
    printf("armemu(strlen(\"%s\")) = %d\n", s ,r);
    arm_metrics_print(&state);
    cache_state_stats(&state.cstate);
    arm_state_free(&state);
}

void single_step_mode(int argc, char *argv[]){
    if(strcmp(argv[1], "-s") != 0 || argc != 3){
        printf("To enter single step mode, input -s with a string without blank\n");
        return;
    }
    test_strlen(argv[2], 1);
}

int main(int argc, char *argv[]){
    if(argc > 1){
        single_step_mode(argc, argv);
        return 0;
    }
    test_strlen("Hello World!", 0);
    test_strlen("No Problem", 0);
    test_strlen("What's up", 0);
    test_strlen("Nice!!", 0);
    return 0;
}
