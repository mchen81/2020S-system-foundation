# project03-mchen81
ARM Assembly Programming

Due Monday, March 2nd, 2020 at 11:59pm in your Github repository for Project03.
Note: Project03 is due Monday night and we will do interactive grading on Tuesday, March 3rd.

In this project you are going to write some ARM assembly functions by hand.

In order to facilitate testing, we will use a C program to call out to the ARM assembly functions.

I have provided an example of the C driver program in the inclass repo.

All of your ARM assembly functions should follow the ARM Procedure Call Conventions as described in class and in the ARM documentation.

Please do not use the compiler to generate ARM assembly output and then turn in this output. It will be pretty obvious if the code you submit appears to be compiler generated. You will get zero points for any program in which you do this.

I provide some guidelines for testing below. You should provide testing output that makes it clear the inputs and resulting outputs so that we can easily see that the tests are working.

You also need to include  Makefile that can both build and test your code.

All of your ARM programs should use the ARMv6 instruction set and they should run on your Raspberry Pi.

Your ARM versions of the programs below should be named with "_s" at the end of the function name. For example, your assembly version of quadratic_c() should be called quadratic_s().

Program 1: quadratic (equation)

Write a function to compute the quadratic equation.

Here is a C implementation:

int quadratic_c(int x, int a, int b, int c)
{
    int result = (a * (x * x)) + (b * x) + c;

    return result;
}

Construct at least 4 different tests with different values. Please include positive and negative values as well as zero. Include some simple examples that are easy to compute in your head.

Compare the output of the C version to your version.

Program 2: sum_array

Sum the elements of an integer array. Assume the elements can be 2's complement signed values.

Here is a C implementation:

int sum_array_c(int *array, int n)
{
    int i;
    int sum = 0;

    for (i = 0; i < n; i++) {
        sum = sum + array[i];
    }

    return sum;
}

Construct 4 different tests with positive, negative, and zero values. You should have a test that sums at least 1000 elements.

Compare the output of the C version to your version.

Program 3: find_max

Find the maximum value in a an array of signed integer values. You can assume this function is called with an array that has at least one element.

Here is a C implementation:

int find_max_c(int *array, int len)
{
    int i;
    int max = array[0];

    for (i = 1; i < len; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }

    return max;
}
Construct 4 different tests with positive, negative, and zero values. You should have a test that has an input array of at least 1000 elements.

Compare the output of the C version to your version.


Program 4: Simple Sort (Descending)


int find_max_index_c(int *array, int len)
{
    int i;
    int max = array[0];
    int index = 0;

    for (i = 1; i < len; i++) {
        if (array[i] > max) {
            max = array[i];
            index = i;
        }
    }

    return index;
}

int sort_c(int *array, int len)
{
    int i;
    int max_index;
    int tmp;

    for (i = 0; i < len; i++) {
        max_index = find_max_index_c(&array[i], len - i) + i;
        if (max_index != i) {
            tmp = array[i];
            array[i] = array[max_index];
            array[max_index] = tmp;
        }

    }

    return len;
}

In this program you must implement your assembly code using two functions just like the C code above. This will get you familiar with how to call an assembly function from an assembly function, which will be helpful for the recursion problem below. Construct 4 different tests with positive, negative, and zero values. You should have a test that has an input array of at least 1000 elements.

Program 5: fib_iter

Implement the fibonacci sequence function iteratively. Given a value n, find the nth fibonacci sequence value. Assume the sequence starts with 0.


int fib_iter_c(int n) {
    int prev_prev_num = 0, prev_num = 0, cur_num = 1;

    if (n == 0) {
        return 0;
    }
    for (int i = 1; i < n ; i++) {
        prev_prev_num = prev_num;
        prev_num = cur_num;
        cur_num = prev_prev_num + prev_num;
    }
    return cur_num;
}

Your test should display the first 20 fibonacci numbers. You should compare your Assembly output to the C output.


Program 6: fib_rec

Implement the fibonacci sequence function recursively. Given a value n, find the nth fibonacci sequence value. Assume the sequence starts with 0.

Here is a C implementation:

int fib_rec_c(int n)
{
    if (n == 0) {
        return 0;
    } else if (n == 1) {
        return 1;
    } else {
        return fib_rec_c(n - 1) + fib_rec_c(n - 2);
    }
}

Your test should display the first 20 fibonacci numbers. You should compare your Assembly output to the C output.

Program 7: strlen

Write a function that will determine the length of a C string:

int strlen_c(char *s)
{
    int i = 0;

    while (s[i] != '\0') {
        i++;
    }

    return i;
}

Provide the ARM version and at least 5 tests.
