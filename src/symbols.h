#ifndef SYMBOLS_H
#define SYMBOLS_H

typedef struct {
    char **names;
    int *addresses;
    int count;
    int capacity;
} SymbolTable;

SymbolTable* symbols_new(void);
void symbols_free(SymbolTable *st);
int symbols_add(SymbolTable *st, const char *name, int address);
int symbols_find(SymbolTable *st, const char *name);
int symbols_has(SymbolTable *st, const char *name);

#endif
