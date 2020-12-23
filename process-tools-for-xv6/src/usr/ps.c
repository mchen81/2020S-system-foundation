#include "types.h"
#include "stat.h"
#include "user.h"
#include "pinfo.h"

int main(int argc, char *argv[]){
   
    struct pitable pit;
    struct pinfo p;
    int count;
    
    count = psinfo(&pit);
    printf(1, "PID      MEM        NAME\n");
    for(int i =0; i< count; i++){
        p = pit.pinfos[i];
        printf(1, "%d        %d      %s\n", p.pid, p.mem, p.name);
    }

    exit();
}
