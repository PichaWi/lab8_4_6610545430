#include <stdio.h>
#include <stdint.h>
#include "src/encode.h"

int main()
{
    uint32_t code_r = encode_r(0x33, 0, 0x00, 1, 2, 3);
    if (code_r == 0x003100b3)
        printf("PASS encode_r\n");
    else
        printf("FAIL encode_r 0x%08x\n", code_r);

    uint32_t code_i = encode_i(0x13, 0, 1, 0, 5);
    if (code_i == 0x00500093)
        printf("PASS encode_i\n");
    else
        printf("FAIL encode_i 0x%08x\n", code_i);

    uint32_t code_s = encode_s(0x23, 2, 2, 1, 20);
    if (code_s == 0x01412023)
        printf("PASS encode_s\n");
    else
        printf("FAIL encode_s 0x%08x\n", code_s);

    uint32_t code_b = encode_b(0x63, 0, 2, 1, 8);
    if (code_b == 0x00810663)
        printf("PASS encode_b\n");
    else
        printf("FAIL encode_b 0x%08x\n", code_b);

    uint32_t code_j = encode_j(0x6f, 1, 8);
    if (code_j == 0x008000ef)
        printf("PASS encode_j\n");
    else
        printf("FAIL encode_j 0x%08x\n", code_j);

    printf("Encode tests completed\n");
    return 0;
}
