#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <file.s>\n", argv[0]);
        return 1;
    }

    int result = assemble_file(argv[1]);
    return result;
}
