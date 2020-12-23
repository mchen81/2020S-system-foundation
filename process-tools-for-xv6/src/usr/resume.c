#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]){
   
    if(argc != 2){
        printf(1, "Usage: resume file\n");
        exit();
    }
    
    int id = fork();
    
    if(id == 0){
        char *filename;
        filename = argv[1];
        if(resume(filename) < 0){
            printf(1, "resume fail\n");
        }
    }
    
    exit();
}
