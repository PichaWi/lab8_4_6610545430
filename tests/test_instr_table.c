#include <stdio.h>
#include <string.h>
#include "src/instr_table.h"

int main()
{
    struct
    {
        const char *name;
        int opcode;
        int funct3;
        InstrFormat fmt;
    } tests[] = {
        {"add", 0x33, 0x0, FMT_R},
        {"addi", 0x13, 0x0, FMT_I},
        {"beq", 0x63, 0x0, FMT_B},
        {"lw", 0x03, 0x2, FMT_I},
        {"jal", 0x6F, 0, FMT_J},
        {"unknown", 0, 0, 0}};

    int passed = 0;
    for (int i = 0; tests[i].name; i++)
    {
        const InstrSpec *spec = lookup_instr(tests[i].name);
        int test_pass = spec != NULL;
        if (spec)
        {
            test_pass &= spec->opcode == tests[i].opcode;
            test_pass &= spec->funct3 == tests[i].funct3;
            test_pass &= spec->format == tests[i].fmt;
        }

        if (test_pass)
        {
            printf("PASS: %s\n", tests[i].name);
            passed++;
        }
        else
        {
            printf("FAIL: %s (opcode=%x, f3=%x, fmt=%d)\n",
                   tests[i].name, spec ? spec->opcode : 0,
                   spec ? spec->funct3 : 0, spec ? spec->format : 0);
        }
    }
    printf("Instr table: %d/%d tests passed\n", passed, 5);
    return passed != 5;
}
