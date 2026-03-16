#ifndef ENCODE_H
#define ENCODE_H

#include <stdint.h>
#include "instr_table.h"

uint32_t encode_r(int opcode, int funct3, int funct7, int rd, int rs1, int rs2);
uint32_t encode_i(int opcode, int funct3, int rd, int rs1, int imm);
uint32_t encode_s(int opcode, int funct3, int rs2, int rs1, int imm);
uint32_t encode_b(int opcode, int funct3, int rs2, int rs1, int imm);
uint32_t encode_j(int opcode, int rd, int imm);
uint32_t encode_instruction(const InstrSpec *spec, int rd, int rs1, int rs2, int imm);

#endif
