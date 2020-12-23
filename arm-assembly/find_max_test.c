#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int find_max_c(int *array, int len);
int find_max_s(int *array, int len);

void print_array(int *array, int n){
    printf("Array: ");
    for(int i=0;i<n-1;i++){
        printf("%d, ", array[i]);
    }
    printf("%d\n", array[n-1]);
}

void run_c(int *a, int n){
    printf("find_max_c returns: %d\n", find_max_c(a, n));
}

void run_s(int *a, int n){
    printf("find_max_s returns: %d\n", find_max_s(a, n));
}

int main(int argc, char *argv[]){
    
    int pos[] = {100 , 5 , 8 , 101, 350};
    print_array(pos, 5);
    run_c(pos, 5);
    run_s(pos, 5);

    int neg[] = {-99999, -686, -631, -673, -666, -689};
    print_array(neg, 6);
    run_c(neg, 6);
    run_s(neg, 6);

    int mixed[] = {1, 3, 10, -999, 0, -384636, 2873264, 9, 0 , 22222};
    print_array(mixed, 10);
    run_c(mixed, 10);
    run_s(mixed, 10);

    int rand_arr[1024];
    int i;
    int r;

    srand(time(0));
    for(i = 0; i< 1024; i++){
        r = rand();
        rand_arr[i] = r;
    }

    printf("Random Array Test...\n");
    run_c(rand_arr, 1024);
    run_s(rand_arr, 1024);

    return 0;
}
