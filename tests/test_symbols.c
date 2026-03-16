#include <stdio.h>
#include "src/symbols.h"

int main()
{
    SymbolTable *st = symbols_new();

    // Test add and find
    symbols_add(st, "loop", 12);
    symbols_add(st, "start", 0);

    int passed = 0, total = 0;

    total++;
    if (symbols_find(st, "loop") == 12)
        passed++;
    total++;
    if (symbols_find(st, "start") == 0)
        passed++;
    total++;
    if (symbols_find(st, "missing") == -1)
        passed++;
    total++;
    if (symbols_add(st, "loop", 99) == -1)
        passed++; // duplicate

    symbols_free(st);
    printf("Symbols: %d/%d tests passed\n", passed, total);
    return passed != total;
}
