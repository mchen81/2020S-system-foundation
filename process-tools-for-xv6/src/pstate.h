
struct pstate{
    int sz; // the size of the target process
    uint sp_usr; // the user-level stack pointer
    uint lr_usr; // the user-level link register (this is important!)
    uint pc; // the user-level pc
    uint    r0;
    uint    r1;
    uint    r2;
    uint    r3;
    uint    r4;
    uint    r5;
    uint    r6;
    uint    r7;
    uint    r8;
    uint    r9;
    uint    r10;
    uint    r11;
    uint    r12;    
    int tracing;
    int code_data_size;
    char name[16];
};
