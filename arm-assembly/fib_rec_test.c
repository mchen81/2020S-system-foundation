#include<stdio.h>

int fib_rec_c(int n);
int fib_rec_s(int n);

void run_c(int n){
    int c = fib_rec_c(n);
    printf("The fibonacci of %d found by C is %d\n", n ,c);
}

void run_s(int n){
    int s = fib_rec_s(n);
    printf("The fibonacci of %d found by S is %d\n", n ,s);
}

int main(int argc, char *argv[]){
    for(int i = 0; i<= 20; i++){
        run_c(i);
        run_s(i);
        printf("=================================================\n");
    }

    return 0;

}
