#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "registers.h"

int parse_register(const char *name) {
    if (strlen(name) < 2 || name[0] != 'x') return -1;
    
    char *endptr;
    int reg = strtol(name + 1, &endptr, 10);
    
    if (*endptr != '\0' || reg < 0 || reg > 31) return -1;
    return reg;
}
