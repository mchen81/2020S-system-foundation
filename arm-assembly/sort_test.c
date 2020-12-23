#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int sort_c(int *array, int len);
int sort_s(int *array, int len);

void print_array(int *array, int n){;
    for(int i=0;i<n-1;i++){
        printf("%d, ", array[i]);
    }
    printf("%d\n", array[n-1]);
}

void run_c(int *a, int n){
    sort_c(a, n);
    printf("Sorted by C : ");
    print_array(a, n);
}

void run_s(int *a, int n){
    sort_s(a, n);
    printf("Sorted by S : ");
    print_array(a, n);
}


int main(int argc, char *argv[]){

    int ary1_1[] = {1, 3, -8, 11, -3, 66, 0};
    int ary1_2[] = {1, 3, -8, 11, -3, 66, 0 };
    printf("Unsorted Array: ");
    print_array(ary1_1, 7);
    run_c(ary1_1, 7);
    run_s(ary1_2, 7);

    int ary2_1[] = {-11, -3, -1, -5, -9, -7};
    int ary2_2[] = {-11, -3, -1, -5, -9, -7};
    printf("Unsorted Array: ");
    print_array(ary2_1, 6);
    run_c(ary2_1, 6);
    run_s(ary2_2, 6);

    int ary3_1[] = {20, 1, 3, -1, -8, 0, 20, -7, 9, 10};
    int ary3_2[] = {20, 1, 3, -1, -8, 0, 20, -7, 9, 10};
    printf("Unsorted Array: ");
    print_array(ary3_1, 10);
    run_c(ary3_1, 10);
    run_s(ary3_2, 10);

    int rand_arr1[1024];
    int rand_arr2[1024];
    int i;
    int r;

    srand(time(0));

    for(i = 0; i< 1024; i++){
        r = rand();
        rand_arr1[i] = r;
        rand_arr2[i] = r;
    }

    printf("Random Array Test...\n");
    sort_c(rand_arr1, 1024);
    sort_s(rand_arr2, 1024);
    
    int is_equal_array = 1;
    for(i =0; i< 1024; i++){
        if(rand_arr1[i] != rand_arr2[i]){
            is_equal_array = -1;
        }
    }
    printf("Is the result of C the same as S? : %s\n", is_equal_array ? "Yes" : "No" );

    return 0;
}
