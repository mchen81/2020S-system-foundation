#include<stdio.h>

int strlen_s(char *s);

void run_s(char *s){
    printf("The length of \"%s\" is %d\n", s, strlen_s(s));
}

int main(int argc, char *argv[]){
    
    char *s1 = "Hello World";
    run_s(s1);

    char *s2 = "My name's Meng-Ze Chen";
    run_s(s2);

    char *s3 = "Nice to meet you";
    run_s(s3);

    char *s4 = "I love CS631";
    run_s(s4);

    char *s5 = "Bye Bye :D";
    run_s(s5);
    
    return 0;
}
