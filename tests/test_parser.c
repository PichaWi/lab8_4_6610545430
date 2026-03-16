#include <stdio.h>
#include <string.h>
#include "src/parser.h"

int main()
{
    struct
    {
        const char *input;
        const char *label;
        const char *mnemonic;
        const char *op1, *op2, *op3;
    } tests[] = {
        {"loop: add x1, x2, x3", "loop", "add", "x1", "x2", "x3"},
        {"  addi x1, x0, 5  ", NULL, "addi", "x1", "x0", "5"},
        {"beq x1 x2 target", NULL, "beq", "x1", "x2", "target"},
        {"main:", "main", NULL, NULL, NULL, NULL},
        {"", NULL, NULL, NULL, NULL, NULL}};

    int passed = 0;
    for (int i = 0; tests[i].input; i++)
    {
        ParsedLine pl = parse_line(tests[i].input);

        int test_pass = 1;
        if (tests[i].label)
        {
            test_pass &= pl.label && !strcmp(pl.label, tests[i].label);
        }
        else
        {
            test_pass &= pl.label == NULL;
        }
        test_pass &= pl.mnemonic &&
                     (!tests[i].mnemonic || !strcmp(pl.mnemonic, tests[i].mnemonic));

        if (tests[i].op1)
        {
            test_pass &= pl.operand_count >= 1 &&
                         !strcmp(pl.operands[0], tests[i].op1);
        }

        if (test_pass)
        {
            printf("PASS: %s\n", tests[i].input);
            passed++;
        }
        else
        {
            printf("FAIL: %s\n", tests[i].input);
            printf("  Got: label='%s', mnemonic='%s', ops=%d\n",
                   pl.label ? pl.label : "NULL",
                   pl.mnemonic ? pl.mnemonic : "NULL", pl.operand_count);
        }
        free_parsed_line(&pl);
    }
    printf("Parser: %d tests passed\n", passed);
    return passed != 4; // 4 valid test cases
}
