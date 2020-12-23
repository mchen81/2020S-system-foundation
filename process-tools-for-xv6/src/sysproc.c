#include "types.h"
#include "arm.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "pinfo.h"
#include "pstate.h"

// projcet7
// part1
int sys_trace_on(void){
    proc->tracing = 1;
    return 0;
}

void tracing(char *name){
    if(proc->tracing){
        cprintf("[%d] %s()\n", proc->pid, name);
    }
}

// part2
int sys_psinfo(void){
    
    struct pitable *pit;
    if(argptr(0, (void *) &pit, sizeof(struct pitable)) < 0){
        return -1;
    }

    tracing("sys_psinfo");
    return psinfo(pit);
}

// part3
int sys_resume(void){
    
    char *filename;
    
    if(argstr(0, &filename) < 0){
        return -1;
    }
    
    if(resume(filename) < 0){
        return -1;
    }
    return proc->pid;
}

// project7 - end

int sys_fork(void)
{   
    tracing("sys_fork");
    return fork();
}

int sys_exit(void)
{
    tracing("sys_exit");
    exit();
    return 0;  // not reached
}

int sys_wait(void)
{
    tracing("sys_wait");
    return wait();
}

int sys_kill(void)
{
    int pid;

    if(argint(0, &pid) < 0) {
        return -1;
    }

    if(proc->tracing){
        cprintf("[%d] %s(%d)\n", proc->pid, "sys_kill", pid);
    }

    return kill(pid);
}

int sys_getpid(void)
{
    tracing("sys_getpid");
    return proc->pid;
}

int sys_sbrk(void)
{
    int addr;
    int n;

    if(argint(0, &n) < 0) {
        return -1;
    }

    addr = proc->sz;

    if(growproc(n) < 0) {
        return -1;
    }

    if(proc->tracing){
        cprintf("[%d] %s(%d)\n", proc->pid, "sys_sbrk", n);
    }

    return addr;
}

int sys_sleep(void)
{
    int n;
    uint ticks0;

    if(argint(0, &n) < 0) {
        return -1;
    }

    acquire(&tickslock);

    ticks0 = ticks;

    while(ticks - ticks0 < n){
        if(proc->killed){
            release(&tickslock);
            return -1;
        }

        sleep(&ticks, &tickslock);
    }

    release(&tickslock);

    if(proc->tracing){
        cprintf("[%d] %s(%d)\n", proc->pid, "sys_sleep", n);
    }

    return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void)
{
    uint xticks;

    acquire(&tickslock);
    xticks = ticks;
    release(&tickslock);
    
    tracing("sys_uptime");

    return xticks;
}

int sys_memfree(void){
    tracing("sys_memfree");
    return kmemfree();
}

