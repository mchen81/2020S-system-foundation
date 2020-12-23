/* utils.c - general utility functions for armemu */

#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

/* malloc helper */
void *malloc_helper(size_t bytes, char *function_name){
    void *p;

    p = malloc(bytes);
    if (p == NULL) {
        printf("%s: malloc() failed, exiting.", function_name);
    }

    return p;
}

unsigned int set_bit(unsigned int unint, int bit){
    unint |= 1 << bit;
    return unint;
}

unsigned int clear_bit(unsigned int unint, int bit){
    unint &= ~(1 << bit);
    return unint;
}

unsigned int get_bit(unsigned int unint, int bit){
    return (unint >> bit) & 0b1;
}

void print_array(int *array, int n){
    printf("Array: ");
    if(n > 20){
        printf("%d, %d, %d ...... %d, %d\n", array[0], array[1], array[2], array[n-2], array[n-1]);
    }else{
        for(int i =0; i < n; i++){
            printf("%d", array[i]);
            if(i < n-1){
                printf(", ");
            }
        }
        printf("\n");
    }
}
