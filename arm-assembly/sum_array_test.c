#include<stdio.h>
#include<stdlib.h>
#include<time.h> 

int sum_array_c(int *array, int n);
int sum_array_s(int *array, int n);

void print_array(int *array, int n){
    printf("Array: ");
    for(int i=0;i<n-1;i++){
        printf("%d, ", array[i]);
    }
    printf("%d\n", array[n-1]); 
}

void run_c(int *a, int n){
    printf("sum_array_c returns: %d\n", sum_array_c(a, n));
}

void run_s(int *a, int n){
    printf("sum_array_s returns : %d\n", sum_array_s(a, n));
}


int main(int argc, char *argv[]){
    
    int pos[] = {1 , 3 , 5 , 7, 9};
    print_array(pos, 5);
    run_c(pos, 5);
    run_s(pos, 5);
    
    int neg[] = {0, -3, -1, -5, -9, -7};
    print_array(neg, 6);
    run_c(neg, 6);
    run_s(neg, 6);

    int mixed[] = {1, 3, -1, -8, 0, 20, -7, 9, 10};
    print_array(mixed, 9);
    run_c(mixed, 9);
    run_s(mixed, 9);

    int rand_arr[1024];
    int i;

    srand(time(0));
    int rand_num;
    for(i = 0; i< 1024; i++){
        rand_num = rand();
        rand_arr[i] = rand_num;
    }

    printf("Random Array Test...\n");
    run_c(rand_arr, 1024);
    run_s(rand_arr, 1024);
    
    return 0;
}


