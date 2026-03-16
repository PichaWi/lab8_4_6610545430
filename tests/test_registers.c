#include <stdio.h>
#include "src/registers.h"

int main()
{
    struct
    {
        const char *name;
        int expected;
    } tests[] = {
        {"x0", 0}, {"x1", 1}, {"x10", 10}, {"x31", 31}, {"x32", -1}, {"x-1", -1}, {"y5", -1}, {"x5a", -1}, {NULL, 0}};

    int passed = 0, total = 0;
    for (int i = 0; tests[i].name; i++)
    {
        total++;
        int result = parse_register(tests[i].name);
        if (result == tests[i].expected)
        {
            printf("PASS: %s -> %d\n", tests[i].name, result);
            passed++;
        }
        else
        {
            printf("FAIL: %s -> %d (expected %d)\n",
                   tests[i].name, result, tests[i].expected);
        }
    }
    printf("Registers: %d/%d tests passed\n", passed, total);
    return total != passed;
}
