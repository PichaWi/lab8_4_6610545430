#ifndef INSTR_TABLE_H
#define INSTR_TABLE_H

typedef enum {
    FMT_R, FMT_I, FMT_S, FMT_B, FMT_U, FMT_J
} InstrFormat;

typedef struct {
    const char *name;
    InstrFormat format;
    int opcode;
    int funct3;
    int funct7;
} InstrSpec;

const InstrSpec* lookup_instr(const char *name);

#endif
