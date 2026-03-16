#include <stdio.h>
#include "src/assembler.h"

int main()
{
    // Create a simple test file
    FILE *testfile = fopen("test.s", "w");
    fprintf(testfile,
            "start:\n"
            "addi x1, x0, 5\n"
            "addi x2, x0, 10\n"
            "loop: add x3, x1, x2\n"
            "beq x3, x2, loop\n");
    fclose(testfile);

    printf("Testing assembler...\n");
    int result = assemble_file("test.s");

    // Check if output file was created
    FILE *outfile = fopen("a.out", "rb");
    if (outfile)
    {
        fseek(outfile, 0, SEEK_END);
        long size = ftell(outfile);
        fclose(outfile);
        printf("PASS: Generated %ld bytes\n", size);
        remove("a.out");
    }
    else
    {
        printf("FAIL: No output generated\n");
    }

    remove("test.s");
    return result != 0;
}
