#include <stdint.h>
#include "encode.h"

uint32_t encode_r(int opcode, int funct3, int funct7, int rd, int rs1, int rs2)
{
    uint32_t instr = 0;
    instr |= (opcode & 0x7F) << 0;
    instr |= (rd & 0x1F) << 7;
    instr |= (funct3 & 0x7) << 12;
    instr |= (rs1 & 0x1F) << 15;
    instr |= (rs2 & 0x1F) << 20;
    instr |= (funct7 & 0x7F) << 25;
    return instr;
}

uint32_t encode_i(int opcode, int funct3, int rd, int rs1, int imm)
{
    uint32_t instr = 0;
    instr |= (opcode & 0x7F) << 0;
    instr |= (rd & 0x1F) << 7;
    instr |= (funct3 & 0x7) << 12;
    instr |= (rs1 & 0x1F) << 15;
    instr |= (imm & 0xFFF) << 20;
    return instr;
}

uint32_t encode_s(int opcode, int funct3, int rs2, int rs1, int imm)
{
    uint32_t instr = 0;
    int imm_low = imm & 0x1F;
    int imm_high = (imm >> 5) & 0x7F;

    instr |= (opcode & 0x7F) << 0;
    instr |= (imm_low & 0x1F) << 7;
    instr |= (funct3 & 0x7) << 12;
    instr |= (rs1 & 0x1F) << 15;
    instr |= (rs2 & 0x1F) << 20;
    instr |= (imm_high & 0x7F) << 25;
    return instr;
}

uint32_t encode_b(int opcode, int funct3, int rs2, int rs1, int imm)
{
    uint32_t instr = 0;
    // B-type immediate: imm[12|10:5|4:1|11]
    int imm_12 = (imm >> 12) & 1;
    int imm_105 = (imm >> 5) & 0x3F;
    int imm_41 = (imm >> 1) & 0xF;
    int imm_11 = (imm >> 11) & 1;

    instr |= (opcode & 0x7F) << 0;
    instr |= (imm_41 & 0xF) << 7; // imm[4:1]
    instr |= (funct3 & 0x7) << 12;
    instr |= (rs1 & 0x1F) << 15;
    instr |= (rs2 & 0x1F) << 20;
    instr |= (imm_105 & 0x3F) << 25; // imm[10:5]
    instr |= (imm_11 & 1) << 31;     // imm[11]
    instr |= (imm_12 & 1) << 7;      // imm[12] in bit 7 position
    return instr;
}

uint32_t encode_j(int opcode, int rd, int imm)
{
    uint32_t instr = 0;
    // J-type: imm[20|10:1|11|19:12]
    int imm_20 = (imm >> 20) & 1;
    int imm_101 = (imm >> 1) & 0x3FF;
    int imm_11 = (imm >> 11) & 1;
    int imm_1912 = (imm >> 12) & 0xFF;

    instr |= (opcode & 0x7F) << 0;
    instr |= (rd & 0x1F) << 7;
    instr |= (imm_1912 & 0xFF) << 12;
    instr |= (imm_11 & 1) << 20;
    instr |= (imm_101 & 0x3FF) << 21;
    instr |= (imm_20 & 1) << 31;
    return instr;
}

uint32_t encode_instruction(const InstrSpec *spec, int rd, int rs1, int rs2, int imm)
{
    if (!spec)
        return 0;

    switch (spec->format)
    {
    case FMT_R:
        return encode_r(spec->opcode, spec->funct3, spec->funct7, rd, rs1, rs2);
    case FMT_I:
        return encode_i(spec->opcode, spec->funct3, rd, rs1, imm);
    case FMT_S:
        return encode_s(spec->opcode, spec->funct3, rs2, rs1, imm);
    case FMT_B:
        return encode_b(spec->opcode, spec->funct3, rs2, rs1, imm);
    case FMT_J:
        return encode_j(spec->opcode, rd, imm);
    default:
        return 0;
    }
}
