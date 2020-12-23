struct pinfo{
    int pid;
    int mem;
    char name[16];
};

struct pitable{
    struct pinfo pinfos[64];
};
