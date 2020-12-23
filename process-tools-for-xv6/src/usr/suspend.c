#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstate.h"

int main(int argc, char *argv[]){
    
    if(argc != 3){
        printf(1, "Wrong arguments\n");
        exit();
    }
    
    int pid, suspended;
    char *filename;

    pid = atoi(argv[1]);
    filename = argv[2];

    suspended = suspend(pid, filename);

    if(suspended < 0){
        printf(1, "fail to suspend %d\n", pid);
    }

    exit();
}
