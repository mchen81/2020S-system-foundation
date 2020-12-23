#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#include"utils.h"
#include"arm.h"

int find_max_c(int *array, int len);
int find_max_s(int *array, int len);

void test_c(int *a, int n){
    printf("find_max_c = %d\n", find_max_c(a, n));
}

void test_s(int *a, int n){
    printf("find_max_s = %d\n", find_max_s(a, n));
}

void test_e(int *array, int n, int single_step){
    struct arm_state state;
    arm_state_init(&state, (unsigned int * )find_max_s, (unsigned int)array, n, 0, 0);
    unsigned int r = armemu(&state, single_step);
    printf("find_max_e = %d\n", r);
    arm_metrics_print(&state);
    cache_state_stats(&state.cstate);
    arm_state_free(&state);
}

void single_step_mode(int argc, char *argv[]){
    if(strcmp(argv[1], "-s") != 0 || argc < 3){
        printf("To enter single step mode, input -s with array elements\n");
        return;
    }
    int size = argc - 2;
    int array[size];
    for(int i = 0; i < size; i++){
        array[i] = atoi(argv[i + 2]);
    }
    print_array(array, size);
    test_e(array, size, 1);
}

int main(int argc, char *argv[]){

    if(argc > 1){
        single_step_mode(argc, argv);
        return 0;
    }


    int pos[] = {100, 5, 8, 101, 350};
    print_array(pos, 5);
    test_c(pos, 5);
    test_s(pos, 5);
    test_e(pos, 5, 0);

    int neg[] = {-99999, -686, -631, -673, -666, -689};
    print_array(neg, 6);
    test_c(neg, 6);
    test_s(neg, 6);
    test_e(neg, 6, 0);

    int mixed[] = {1, 3, 10, -999, 0, -384636, 2873264, 9, 0, 22222};
    print_array(mixed, 10);
    test_c(mixed, 10);
    test_s(mixed, 10);
    test_e(mixed, 10, 0);

    int rand_arr[1024];
    int i;
    int r;
    srand(time(0));
    for(i = 0; i< 1024; i++){
        r = rand();
        rand_arr[i] = r;
    }

    printf("Random Array Test...\n");
    print_array(rand_arr, 1024);
    test_c(rand_arr, 1024);
    test_s(rand_arr, 1024);
    test_e(rand_arr, 1024, 0);

    return 0;
}
