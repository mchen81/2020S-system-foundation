#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#include"utils.h"
#include"arm.h"

int sort_c(int *array, int len);
int sort_s(int *array, int len);

void test_c(int *array, int n){
    sort_c(array, n);
    printf("Sorted by C: ");
    print_array(array, n);
}

void test_s(int *array, int n){
    sort_s(array, n);
    printf("Sorted by S: ");
    print_array(array, n);
}

void test_e(int *array, int n, int single_step){
    struct arm_state state;
    arm_state_init(&state, (unsigned int * )sort_s, (unsigned int)array, n, 0, 0);
    armemu(&state, single_step);
    printf("Sorted by E: ");
    print_array(array, n);
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

    int ary1_1[] = {1, 3, -8, 11, -3, 66, 0};
    int ary1_2[] = {1, 3, -8, 11, -3, 66, 0 };
    int ary1_3[] = {1, 3, -8, 11, -3, 66, 0 };
    printf("Unsorted ");
    print_array(ary1_1, 7);
    test_c(ary1_1, 7);
    test_s(ary1_2, 7);
    test_e(ary1_3, 7, 0);

    int ary2_1[] = {-11, -3, -1, -5, -9, -7};
    int ary2_2[] = {-11, -3, -1, -5, -9, -7};
    int ary2_3[] = {-11, -3, -1, -5, -9, -7};
    printf("Unsorted ");
    print_array(ary2_1, 6);
    test_c(ary2_1, 6);
    test_s(ary2_2, 6);
    test_e(ary2_3, 6, 0);

    int ary3_1[] = {20, 1, 3, -1, -8, 0, 20, -7, 9, 10};
    int ary3_2[] = {20, 1, 3, -1, -8, 0, 20, -7, 9, 10};
    int ary3_3[] = {20, 1, 3, -1, -8, 0, 20, -7, 9, 10};
    printf("Unsorted ");
    print_array(ary3_1, 10);
    test_c(ary3_1, 10);
    test_s(ary3_2, 10);
    test_e(ary3_3, 10, 0);

    int rand_arr1[1024];
    int rand_arr2[1024];
    int rand_arr3[1024];
    int i;
    int r;

    srand(time(0));
    for(i = 0; i< 1024; i++){
        r = rand();
        rand_arr1[i] = r;
        rand_arr2[i] = r;
        rand_arr3[i] = r;
    }

    printf("Unsorted ");
    print_array(rand_arr3, 1024);
    test_c(rand_arr1, 1024);
    test_s(rand_arr2, 1024);
    test_e(rand_arr3, 1024, 0);

    int is_equal_array = 1;
    for(i =0; i< 1024; i++){
        if(rand_arr1[i] != rand_arr2[i] || rand_arr1[i] != rand_arr3[i] || rand_arr2[i] != rand_arr3[i]){
            is_equal_array = -1;
        }
    }

    printf("Are the results of three identical:  %s\n", is_equal_array ? "Yes" : "No" );

    return 0;
}
