#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]){
    int id;
    
    id = fork();
    
    if(argc < 2){
        printf(1, "[Wrong Arguments] strace <program name> <program arguments>");
        exit();
    }

    if(id == 0){
        trace_on();
        exec(argv[1], &argv[1]);
        printf(1, "Cannot execute %s\n", argv[1]);
        exit();
    }else {
        wait();
    }

    exit();
}
