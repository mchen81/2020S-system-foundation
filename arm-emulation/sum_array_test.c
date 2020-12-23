#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#include"utils.h"
#include"arm.h"

int sum_array_c(int *array, int n);
int sum_array_s(int *array, int n);

void test_c(int *array, int n){
    printf("sum_array_c : %d\n", sum_array_c(array, n));
}

void test_s(int *array, int n){
    printf("sum_array_s : %d\n", sum_array_s(array, n));
}

void test_e(int *array, int n, int single_step){
    struct arm_state state;
    arm_state_init(&state, (unsigned int * )sum_array_s, (unsigned int)array, n, 0, 0);
    unsigned int r = armemu(&state, single_step);
    printf("sum_array_e : %d\n", r);
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

    int pos[] = {1 , 3 , 5 , 7, 9};
    print_array(pos, 5);
    test_c(pos, 5);
    test_s(pos, 5);
    test_e(pos, 5, 0);

    int neg[] = {0, -3, -1, -5, -9, -7};
    print_array(neg, 6);
    test_c(neg, 6);
    test_s(neg, 6);
    test_e(neg, 6, 0);


    int mixed[] = {1, 3, -1, -8, 0, 20, -7, 9, 10};
    print_array(mixed, 9);
    test_c(mixed, 9);
    test_s(mixed, 9);
    test_e(mixed, 9, 0);


    int rand_arr[1024];
    int i;

    srand(time(0));
    int rand_num;
    for(i = 0; i< 1024; i++){
        rand_num = rand();
        rand_arr[i] = rand_num;
    }

    printf("Random Array Test (Size 1024)...\n");
    print_array(rand_arr, 1024);
    test_c(rand_arr, 1024);
    test_s(rand_arr, 1024);
    test_e(rand_arr, 1024, 0);

    return 0;
}
