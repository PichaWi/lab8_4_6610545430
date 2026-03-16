#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols.h"

SymbolTable *symbols_new(void)
{
    SymbolTable *st = malloc(sizeof(SymbolTable));
    st->names = malloc(32 * sizeof(char *));
    st->addresses = malloc(32 * sizeof(int));
    st->count = 0;
    st->capacity = 32;
    return st;
}

void symbols_free(SymbolTable *st)
{
    for (int i = 0; i < st->count; i++)
    {
        free(st->names[i]);
    }
    free(st->names);
    free(st->addresses);
    free(st);
}

int symbols_add(SymbolTable *st, const char *name, int address)
{
    if (symbols_has(st, name))
        return -1; // duplicate

    if (st->count >= st->capacity)
    {
        st->capacity *= 2;
        st->names = realloc(st->names, st->capacity * sizeof(char *));
        st->addresses = realloc(st->addresses, st->capacity * sizeof(int));
    }

    st->names[st->count] = strdup(name);
    st->addresses[st->count] = address;
    st->count++;
    return 0;
}

int symbols_find(SymbolTable *st, const char *name)
{
    for (int i = 0; i < st->count; i++)
    {
        if (!strcmp(st->names[i], name))
        {
            return st->addresses[i];
        }
    }
    return -1;
}

int symbols_has(SymbolTable *st, const char *name)
{
    return symbols_find(st, name) != -1;
}
