/* utils.h - Public functions in utils.c */

#include <stddef.h>

#ifndef __utils
    extern void *malloc_helper(size_t bytes, char *function_name);
    extern unsigned int set_bit(unsigned int unint, int bit);
    extern unsigned int clear_bit(unsigned int unint, int bit);
    extern unsigned int get_bit(unsigned int unint, int bit);
    extern void print_array(int *array, int n);
#endif
