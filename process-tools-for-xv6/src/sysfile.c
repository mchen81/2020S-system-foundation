//
// File-system system calls.
// Mostly argument checking, since we don't trust
// user code, and calls into file.c and fs.c.
//

#include "types.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "mmu.h"
#include "proc.h"
#include "fs.h"
#include "file.h"
#include "fcntl.h"
#include "pstate.h"
#include "arm.h"

// Fetch the nth word-sized system call argument as a file descriptor
// and return both the descriptor and the corresponding struct file.
static int argfd(int n, int *pfd, struct file **pf)
{
    int fd;
    struct file *f;

    if(argint(n, &fd) < 0) {
        return -1;
    }

    if(fd < 0 || fd >= NOFILE || (f=proc->ofile[fd]) == 0) {
        return -1;
    }

    if(pfd) {
        *pfd = fd;
    }

    if(pf) {
        *pf = f;
    }

    return 0;
}

// Allocate a file descriptor for the given file.
// Takes over file reference from caller on success.
static int fdalloc(struct file *f)
{
    int fd;

    for(fd = 0; fd < NOFILE; fd++){
        if(proc->ofile[fd] == 0){
            proc->ofile[fd] = f;
            return fd;
        }
    }

    return -1;
}

int sys_dup(void)
{
    struct file *f;
    int old_fd, fd;

    if(argfd(0, &old_fd, &f) < 0) {
        return -1;
    }

    if((fd=fdalloc(f)) < 0) {
        return -1;
    }

    filedup(f);
    
    if(proc->tracing){
        cprintf("[%d] sys_dup(%d)\n", proc->pid, old_fd);
    }

    return fd;
}

int sys_read(void)
{
    struct file *f;
    int n;
    char *p;
    int fd;
    if(argfd(0, &fd, &f) < 0 || argint(2, &n) < 0 || argptr(1, &p, n) < 0) {
        return -1;
    }

    if(proc->tracing){
        cprintf("[%d] sys_read(%d, <%x>, %d)\n", proc->pid, fd, p, n);
    }

    return fileread(f, p, n);
}

int sys_write(void)
{
    struct file *f;
    int n;
    char *p;
    int fd;
    if(argfd(0, &fd, &f) < 0 || argint(2, &n) < 0 || argptr(1, &p, n) < 0) {
        return -1;
    }
    
    if(proc->tracing){
        int bk = 0;
        char pp[n+2];

        for(int i = 0, j = 0; i < n; i++, j++){
            if(p[i] == '\n'){
                pp[j++] = '\\';
                pp[j++] = 'n';
                pp[j] = 0;
                bk = 1;
            }else{
                pp[j] = p[i];
            }
        }

        cprintf("[%d] sys_write(%d, \"%s\", %d)\n", proc->pid, fd, bk ? pp : p, n);
    }

    return filewrite(f, p, n);
}

int sys_close(void)
{
    int fd;
    struct file *f;

    if(argfd(0, &fd, &f) < 0) {
        return -1;
    }

    if(proc->tracing){
        cprintf("[%d] sys_close(%d)\n", proc->pid, fd);
    }

    proc->ofile[fd] = 0;
    fileclose(f);

    return 0;
}

int sys_fstat(void)
{
    struct file *f;
    struct stat *st;
    int fd;
    if(argfd(0, &fd, &f) < 0 || argptr(1, (void*)&st, sizeof(*st)) < 0) {
        return -1;
    }

    if(proc->tracing){
        cprintf("[%d] sys_fstat(%d, <%x>)\n", proc->pid, fd, st);
    }


    return filestat(f, st);
}

// Create the path new as a link to the same inode as old.
int sys_link(void)
{
    char name[DIRSIZ], *new, *old;
    struct inode *dp, *ip;

    if(argstr(0, &old) < 0 || argstr(1, &new) < 0) {
        return -1;
    }

    if((ip = namei(old)) == 0) {
        return -1;
    }

    begin_trans();

    ilock(ip);

    if(ip->type == T_DIR){
        iunlockput(ip);
        commit_trans();
        return -1;
    }

    ip->nlink++;
    iupdate(ip);
    iunlock(ip);

    if((dp = nameiparent(new, name)) == 0) {
        goto bad;
    }

    ilock(dp);

    if(dp->dev != ip->dev || dirlink(dp, name, ip->inum) < 0){
        iunlockput(dp);
        goto bad;
    }

    if(proc->tracing){
        cprintf("[%d] sys_link(\"%s\", \"%s\")\n", proc->pid, old, new);
    }


    iunlockput(dp);
    iput(ip);

    commit_trans();
    
    

    return 0;

    bad:
    ilock(ip);
    ip->nlink--;
    iupdate(ip);
    iunlockput(ip);
    commit_trans();
    return -1;
}

// Is the directory dp empty except for "." and ".." ?
static int isdirempty(struct inode *dp)
{
    int off;
    struct dirent de;

    for(off=2*sizeof(de); off<dp->size; off+=sizeof(de)){
        if(readi(dp, (char*)&de, off, sizeof(de)) != sizeof(de)) {
            panic("isdirempty: readi");
        }

        if(de.inum != 0) {
            return 0;
        }
    }
    return 1;
}

//PAGEBREAK!
int sys_unlink(void)
{
    struct inode *ip, *dp;
    struct dirent de;
    char name[DIRSIZ], *path;
    uint off;

    if(argstr(0, &path) < 0) {
        return -1;
    }

    if((dp = nameiparent(path, name)) == 0) {
        return -1;
    }

    begin_trans();

    ilock(dp);

    // Cannot unlink "." or "..".
    if(namecmp(name, ".") == 0 || namecmp(name, "..") == 0) {
        goto bad;
    }

    if((ip = dirlookup(dp, name, &off)) == 0) {
        goto bad;
    }

    ilock(ip);

    if(ip->nlink < 1) {
        panic("unlink: nlink < 1");
    }

    if(ip->type == T_DIR && !isdirempty(ip)){
        iunlockput(ip);
        goto bad;
    }

    memset(&de, 0, sizeof(de));

    if(writei(dp, (char*)&de, off, sizeof(de)) != sizeof(de)) {
        panic("unlink: writei");
    }

    if(ip->type == T_DIR){
        dp->nlink--;
        iupdate(dp);
    }

    iunlockput(dp);

    ip->nlink--;
    iupdate(ip);
    iunlockput(ip);

    commit_trans();
    
    if(proc->tracing){
        cprintf("[%d] sys_unlink(\"%s\")\n", proc->pid, path);
    }

    return 0;

    bad:
    iunlockput(dp);
    commit_trans();
    return -1;
}

static struct inode* create(char *path, short type, short major, short minor)
{
    uint off;
    struct inode *ip, *dp;
    char name[DIRSIZ];

    if((dp = nameiparent(path, name)) == 0) {
        return 0;
    }

    ilock(dp);

    if((ip = dirlookup(dp, name, &off)) != 0){
        iunlockput(dp);
        ilock(ip);

        if(type == T_FILE && ip->type == T_FILE) {
            return ip;
        }

        iunlockput(ip);

        return 0;
    }

    if((ip = ialloc(dp->dev, type)) == 0) {
        panic("create: ialloc");
    }

    ilock(ip);
    ip->major = major;
    ip->minor = minor;
    ip->nlink = 1;
    iupdate(ip);

    if(type == T_DIR){  // Create . and .. entries.
        dp->nlink++;  // for ".."
        iupdate(dp);

        // No ip->nlink++ for ".": avoid cyclic ref count.
        if(dirlink(ip, ".", ip->inum) < 0 || dirlink(ip, "..", dp->inum) < 0) {
            panic("create dots");
        }
    }

    if(dirlink(dp, name, ip->inum) < 0) {
        panic("create: dirlink");
    }

    iunlockput(dp);

    return ip;
}

int sys_open(void)
{
    char *path;
    int fd, omode;
    struct file *f;
    struct inode *ip;

    if(argstr(0, &path) < 0 || argint(1, &omode) < 0) {
        return -1;
    }

    if(omode & O_CREATE){
        begin_trans();
        ip = create(path, T_FILE, 0, 0);
        commit_trans();

        if(ip == 0) {
            return -1;
        }

    } else {
        if((ip = namei(path)) == 0) {
            return -1;
        }

        ilock(ip);

        if(ip->type == T_DIR && omode != O_RDONLY){
            iunlockput(ip);
            return -1;
        }
    }

    if((f = filealloc()) == 0 || (fd = fdalloc(f)) < 0){
        if(f) {
            fileclose(f);
        }

        iunlockput(ip);
        return -1;
    }
    
    iunlock(ip);
    f->type = FD_INODE;
    f->ip = ip;
    f->off = 0;
    f->readable = !(omode & O_WRONLY);
    f->writable = (omode & O_WRONLY) || (omode & O_RDWR);

    if(proc->tracing){
        cprintf("[%d] sys_open(\"%s\", <%x>)\n", proc->pid, path, omode);
    }

    return fd;
}

int sys_mkdir(void)
{
    char *path;
    struct inode *ip;

    begin_trans();

    if(argstr(0, &path) < 0 || (ip = create(path, T_DIR, 0, 0)) == 0){
        commit_trans();
        return -1;
    }

    if(proc->tracing){
        cprintf("[%d] sys_mkdir(\"%s\")\n", proc->pid, path);
    }


    iunlockput(ip);
    commit_trans();

    return 0;
}

int sys_mknod(void)
{
    struct inode *ip;
    char *path;
    int len;
    int major, minor;

    begin_trans();

    if((len=argstr(0, &path)) < 0 ||
            argint(1, &major) < 0 || argint(2, &minor) < 0 ||
            (ip = create(path, T_DEV, major, minor)) == 0){

        commit_trans();
        return -1;
    }


    if(proc->tracing){
        cprintf("[%d] sys_mknod(\"%s\", %d, %d)\n", proc->pid, path, major, minor);
    }

    iunlockput(ip);
    commit_trans();

    return 0;
}

int sys_chdir(void)
{
    char *path;
    struct inode *ip;

    if(argstr(0, &path) < 0 || (ip = namei(path)) == 0) {
        return -1;
    }

    ilock(ip);

    if(ip->type != T_DIR){
        iunlockput(ip);
        return -1;
    }

    
    if(proc->tracing){
        cprintf("[%d] sys_chdir(\"%s\")\n", proc->pid, path);
    }

    iunlock(ip);

    iput(proc->cwd);
    proc->cwd = ip;

    return 0;
}

int sys_exec(void)
{
    char *path, *argv[MAXARG];
    int i;
    uint uargv, uarg;

    if(argstr(0, &path) < 0 || argint(1, (int*)&uargv) < 0){
        return -1;
    }

    memset(argv, 0, sizeof(argv));

    for(i=0;; i++){
        if(i >= NELEM(argv)) {
            return -1;
        }

        if(fetchint(uargv+4*i, (int*)&uarg) < 0) {
            return -1;
        }

        if(uarg == 0){
            argv[i] = 0;
            break;
        }

        if(fetchstr(uarg, &argv[i]) < 0) {
            return -1;
        }
    }

    if(proc->tracing){
        cprintf("[%d] sys_exec(\"%s\", [", proc->pid, path);
        for(int j = 0; j < i; j++){
            cprintf("\"%s\"", argv[j]);
            if(j < i - 1){
                cprintf(",");
            }
        }
        cprintf(",NULL])\n");

    }

    return exec(path, argv);
}

int sys_pipe(void)
{
    int *fd;
    struct file *rf, *wf;
    int fd0, fd1;

    if(argptr(0, (void*)&fd, 2*sizeof(fd[0])) < 0) {
        return -1;
    }

    if(pipealloc(&rf, &wf) < 0) {
        return -1;
    }

    fd0 = -1;

    if((fd0 = fdalloc(rf)) < 0 || (fd1 = fdalloc(wf)) < 0){
        if(fd0 >= 0) {
            proc->ofile[fd0] = 0;
        }

        fileclose(rf);
        fileclose(wf);

        return -1;
    }

    fd[0] = fd0;
    fd[1] = fd1;

    if(proc->tracing){
        cprintf("[%d] sys_pipe(<%x>)\n",proc->pid, fd);
    }

    return 0;
}

//part3
int sys_suspend(void){

    int pid, fd, omode;
    char *filename;

    struct pstate ps;
    struct proc *p;

    struct file *f;
    struct inode *ip;
    
    omode = O_CREATE | O_WRONLY;
    
    // get first param - pid
    if(argint(0, &pid) < 0){
        return -1;
    }
    
    // get second param - filename
    if(argstr(1, &filename) < 0 ){
        return -1;
    }
    
    // get target process p
    if((p = suspend(pid)) == 0){
        return -1;
    }

    // set pstate(header)
    ps.sz = p->sz;
    ps.sp_usr = p->tf->sp_usr;
    ps.lr_usr = p->tf->lr_usr;
    ps.pc = p->tf->pc;
    ps.tracing = p->tracing;
    ps.code_data_size = p->sz - 2 * 4096;
    
    //process regs
    ps.r0 = p->tf->r0;
    ps.r1 = p->tf->r1;
    ps.r2 = p->tf->r2;
    ps.r3 = p->tf->r3;
    ps.r4 = p->tf->r4;
    ps.r5 = p->tf->r5;
    ps.r6 = p->tf->r6;
    ps.r7 = p->tf->r7;
    ps.r8 = p->tf->r8;
    ps.r9 = p->tf->r9;
    ps.r10 = p->tf->r10;
    ps.r11 = p->tf->r11;
    ps.r12 = p->tf->r12;
    
    // process name
    for(int i=0;i < 16; i++){
        ps.name[i] = p->name[i];
    }
    
    // create file
    begin_trans();
    ip = create(filename, T_FILE, 0, 0);
    commit_trans();

    if(ip == 0) {
        return -1;
    }

    if((f = filealloc()) == 0 || (fd = fdalloc(f)) < 0){
        if(f) {
            fileclose(f);
        }

        iunlockput(ip);
        return -1;
    }

    iunlock(ip);
    f->type = FD_INODE;
    f->ip = ip;
    f->off = 0;
    f->readable = !(omode & O_WRONLY);
    f->writable = (omode & O_WRONLY) || (omode & O_RDWR);
    
    switchuvm(p); // switch to user virtual address space for target
    filewrite(f, (char *) &ps, sizeof(ps));         // write header
    filewrite(f, (char *) 0, ps.code_data_size);    // write code+data
    filewrite(f, (char *) (ps.sz - 4096), 4096);    // write stack
    switchuvm(proc); // switch back to the address space of the suspend process
    
    kill(pid);

    if(proc->tracing){
        cprintf("[%d] sys_suspend(%d, \"%s\")", proc->pid, pid, filename);
    }

    return 0;
}
