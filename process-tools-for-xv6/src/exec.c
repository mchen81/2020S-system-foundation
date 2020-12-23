#include "types.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "elf.h"
#include "arm.h"
#include "pstate.h"

// load a user program for execution
int exec (char *path, char **argv)
{
    struct elfhdr elf;
    struct inode *ip;
    struct proghdr ph;
    pde_t *pgdir;
    pde_t *oldpgdir;
    char *s;
    char *last;
    int i;
    int off;
    uint argc;
    uint sz;
    uint sp;
    uint ustack[3 + MAXARG + 1];

    if ((ip = namei(path)) == 0) {
        return -1;
    }

    ilock(ip);

    // Check ELF header
    if (readi(ip, (char*) &elf, 0, sizeof(elf)) < sizeof(elf)) {
        goto bad;
    }

    if (elf.magic != ELF_MAGIC) {
        goto bad;
    }

    pgdir = 0;

    if ((pgdir = kpt_alloc()) == 0) {
        goto bad;
    }
    // Load program into memory.
    sz = 0;

    for (i = 0, off = elf.phoff; i < elf.phnum; i++, off += sizeof(ph)) {
        if (readi(ip, (char*) &ph, off, sizeof(ph)) != sizeof(ph)) {
            goto bad;
        }

        if (ph.type != ELF_PROG_LOAD) {
            continue;
        }

        if (ph.memsz < ph.filesz) {
            goto bad;
        }
        
        if ((sz = allocuvm(pgdir, sz, ph.vaddr + ph.memsz)) == 0) {
            goto bad;
        }

        if (loaduvm(pgdir, (char*) ph.vaddr, ip, ph.off, ph.filesz) < 0) {
            goto bad;
        }
    }

    iunlockput(ip);
    ip = 0;

    // Allocate two pages at the next page boundary.
    // Make the first inaccessible.  Use the second as the user stack.
    
    sz = align_up (sz, PTE_SZ);


    if ((sz = allocuvm(pgdir, sz, sz + 2 * PTE_SZ)) == 0) {
        goto bad;
    }
    
    clearpteu(pgdir, (char*) (sz - 2 * PTE_SZ));

    sp = sz;
    
    // Push argument strings, prepare rest of stack in ustack.
    for (argc = 0; argv[argc]; argc++) {
        if (argc >= MAXARG) {
            goto bad;
        }

        sp = (sp - (strlen(argv[argc]) + 1)) & ~3;

        if (copyout(pgdir, sp, argv[argc], strlen(argv[argc]) + 1) < 0) {
            goto bad;
        }

        ustack[argc] = sp;
    }

    ustack[argc] = 0;

    // in ARM, parameters are passed in r0 and r1
    proc->tf->r0 = argc;
    proc->tf->r1 = sp - (argc + 1) * 4;

    sp -= (argc + 1) * 4;

    if (copyout(pgdir, sp, ustack, (argc + 1) * 4) < 0) {
        goto bad;
    }

    // Save program name for debugging.
    for (last = s = path; *s; s++) {
        if (*s == '/') {
            last = s + 1;
        }
    }

    safestrcpy(proc->name, last, sizeof(proc->name));

    // Commit to the user image.
    oldpgdir = proc->pgdir;
    proc->pgdir = pgdir;
    proc->sz = sz;
    proc->tf->pc = elf.entry;
    proc->tf->sp_usr = sp;

    switchuvm(proc);
    freevm(oldpgdir);
    return 0;

    bad: if (pgdir) {
        freevm(pgdir);
    }

    if (ip) {
        iunlockput(ip);
    }
    return -1;
}

int resume(char *filename){

    struct inode *ip;
    struct pstate ps;
    pde_t *pgdir;
    pde_t *oldpgdir;
    int sz, code_sz;
    // load file to ip
    if((ip = namei(filename)) == 0){
        return -1;
    }

    ilock(ip);

    // read header to ps
    if(readi(ip, (char*) &ps, 0, sizeof(ps)) < sizeof(ps)){
        goto resume_bad;
    }

    pgdir = 0;
    // load pgdir
    if((pgdir = kpt_alloc()) == 0){
        goto resume_bad;
    }

    sz = 0;
    code_sz = ps.code_data_size;

    // allocate code+data size to pgdir
    if((sz = allocuvm(pgdir, 0, code_sz)) == 0){
        goto resume_bad;
    }

    // load code + data
    if(loaduvm(pgdir, 0, ip, sizeof(ps), code_sz) < 0){
        goto resume_bad;
    }

    // allocate 2 * 4096 stack to pgdir
    sz = align_up (sz, PTE_SZ);
    if ((sz = allocuvm(pgdir, sz, sz + 2 * PTE_SZ)) == 0) {
        goto resume_bad;
    }

    //  load stack
    if(loaduvm(pgdir, (char *)(code_sz + 4096), ip, sizeof(ps) + code_sz, 4096) < 0){
        goto resume_bad;
    }

    iunlockput(ip);

    oldpgdir = proc->pgdir;

    proc->pgdir = pgdir;
    proc->sz = ps.sz;
    proc->tf->pc = ps.pc;
    proc->tf->sp_usr =ps.sp_usr;
    proc->tf->lr_usr = ps.lr_usr;
    proc->tracing = ps.tracing;

    for(int i=0; i< 16; i++){
        proc->name[i] = ps.name[i];
    }
    
    proc->tf->r0 = ps.r0;
    proc->tf->r1 = ps.r1;
    proc->tf->r2 = ps.r2;
    proc->tf->r3 = ps.r3;
    proc->tf->r4 = ps.r4;
    proc->tf->r5 = ps.r5;
    proc->tf->r6 = ps.r6;
    proc->tf->r7 = ps.r7;
    proc->tf->r8 = ps.r8;
    proc->tf->r9 = ps.r9;
    proc->tf->r10 = ps.r10;
    proc->tf->r11 = ps.r11;
    proc->tf->r12 = ps.r12;

    switchuvm(proc);
    freevm(oldpgdir);

    return 0;

    resume_bad:
    if(pgdir){
        freevm(pgdir);
    }

    if(ip){
        iunlockput(ip);
    }

    return -1;
}
