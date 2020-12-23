#include<stdio.h>
#include<stdlib.h>

int quadratic_c(int x, int a, int b, int c);
int quadratic_s(int x, int a, int b, int c);

void run_c(int x, int a, int b, int c){
    int r;
    r = quadratic_c(x, a, b, c);
    printf("quadratic_c(%d, %d, %d, %d) returns %d \n", x, a, b, c, r);
}

void run_s(int x, int a, int b, int c){
    int r;
    r = quadratic_s(x, a, b, c);
    printf("quadratic_s(%d, %d, %d, %d) returns %d \n", x, a, b, c, r);
}

int main(int argc, char *argv[]){
    
    int x = 2, a = 3, b = 4, c = 1;
    run_c(x, a, b, c);
    run_s(x, a, b, c);
    
    x = -3;
    a = -1;
    b = -8;
    c = -1;
    run_c(x, a, b, c);
    run_s(x, a, b, c);
    
    x = 0;
    a = -3;
    b = 77;
    c = 8;
    run_c(x, a, b, c);
    run_s(x, a, b, c);
    
    x = 3;
    a = 2;
    b = -2;
    c = -4;
    run_c(x, a, b, c);
    run_s(x, a, b, c);
    
    return 0;
}

