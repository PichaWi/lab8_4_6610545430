#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#include "parser.h"
#include "instr_table.h"
#include "registers.h"
#include "symbols.h"
#include "encode.h"

typedef struct
{
    ParsedLine pl;
    int address;
} LineInfo;

static int is_label_only(const ParsedLine *pl)
{
    return pl->mnemonic == NULL && pl->operand_count == 0;
}

static int parse_operand_to_int(const char *op, SymbolTable *st, int pc, int *result)
{
    if (!op || !result)
        return -1;

    // Try register first
    int reg = parse_register(op);
    if (reg >= 0)
    {
        *result = reg;
        return 0;
    }

    // Try immediate (decimal)
    char *endptr;
    long imm = strtol(op, &endptr, 10);
    if (*endptr == '\0')
    {
        *result = (int)imm;
        return 0;
    }

    // Try symbol lookup
    int addr = symbols_find(st, op);
    if (addr >= 0)
    {
        *result = addr - pc;
        return 0;
    }

    return -1;
}

int assemble_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Failed to open file");
        return -1;
    }

    // Pass 1: Build symbol table
    SymbolTable *st = symbols_new();
    int line_count = 0, capacity = 32;
    LineInfo *lines = malloc(capacity * sizeof(LineInfo));
    if (!lines)
    {
        perror("malloc failed");
        symbols_free(st);
        fclose(file);
        return -1;
    }
    int pc = 0;

    char linebuf[256];
    while (fgets(linebuf, sizeof(linebuf), file))
    {
        ParsedLine pl = parse_line(linebuf);

        // Skip empty lines
        if (!pl.label && !pl.mnemonic)
        {
            free_parsed_line(&pl);
            continue;
        }

        // Handle label-only line
        if (pl.label && !pl.mnemonic)
        {
            if (symbols_add(st, pl.label, pc) < 0)
            {
                fprintf(stderr, "Duplicate label: %s\n", pl.label);
                free_parsed_line(&pl);
                fclose(file);
                symbols_free(st);
                free(lines);
                return -1;
            }
            free_parsed_line(&pl);
            continue;
        }

        // Handle label+instruction
        if (pl.label)
        {
            if (symbols_add(st, pl.label, pc) < 0)
            {
                fprintf(stderr, "Duplicate label: %s\n", pl.label);
                free_parsed_line(&pl);
                fclose(file);
                symbols_free(st);
                free(lines);
                return -1;
            }
        }

        // Store line info
        if (line_count >= capacity)
        {
            capacity *= 2;
            LineInfo *new_lines = realloc(lines, capacity * sizeof(LineInfo));
            if (!new_lines)
            {
                perror("realloc failed");
                free_parsed_line(&pl);
                symbols_free(st);
                free(lines);
                fclose(file);
                return -1;
            }
            lines = new_lines;
        }
        lines[line_count++] = (LineInfo){pl, pc};
        pc += 4; // Each instruction is 4 bytes
    }
    fclose(file);

    // Pass 2: Encode instructions
    FILE *outfile = fopen("a.out", "wb");
    if (!outfile)
    {
        perror("Failed to create output file");
        goto cleanup;
    }

    for (int i = 0; i < line_count; i++)
    {
        LineInfo *li = &lines[i];
        ParsedLine *pl = &li->pl;

        // Skip label-only lines
        if (is_label_only(pl))
        {
            free_parsed_line(pl);
            continue;
        }

        // Lookup instruction
        const InstrSpec *spec = lookup_instr(pl->mnemonic);
        if (!spec)
        {
            fprintf(stderr, "Unknown instruction: %s\n", pl->mnemonic);
            fclose(outfile);
            goto cleanup;
        }

        // Parse operands based on instruction format
        int rd = -1, rs1 = -1, rs2 = -1, imm = 0;
        int ok = 1;

        if (spec->format == FMT_R)
        {
            if (pl->operand_count != 3)
                ok = 0;
            else
                ok &= parse_operand_to_int(pl->operands[0], st, li->address, &rd) == 0;
            ok &= parse_operand_to_int(pl->operands[1], st, li->address, &rs1) == 0;
            ok &= parse_operand_to_int(pl->operands[2], st, li->address, &rs2) == 0;
        }
        else if (spec->format == FMT_I || spec->format == FMT_J)
        {
            if (pl->operand_count != 3)
                ok = 0;
            else
                ok &= parse_operand_to_int(pl->operands[0], st, li->address, &rd) == 0;
            ok &= parse_operand_to_int(pl->operands[1], st, li->address, &rs1) == 0;
            ok &= parse_operand_to_int(pl->operands[2], st, li->address, &imm) == 0;
        }
        else if (spec->format == FMT_S)
        {
            if (pl->operand_count != 3)
                ok = 0;
            else
                ok &= parse_operand_to_int(pl->operands[0], st, li->address, &rs2) == 0;
            ok &= parse_operand_to_int(pl->operands[1], st, li->address, &rs1) == 0;
            ok &= parse_operand_to_int(pl->operands[2], st, li->address, &imm) == 0;
        }
        else if (spec->format == FMT_B)
        {
            if (pl->operand_count != 3)
                ok = 0;
            else
                ok &= parse_operand_to_int(pl->operands[0], st, li->address, &rs1) == 0;
            ok &= parse_operand_to_int(pl->operands[1], st, li->address, &rs2) == 0;
            ok &= parse_operand_to_int(pl->operands[2], st, li->address, &imm) == 0;
        }
        else
        {
            ok = 0;
        }

        if (!ok)
        {
            fprintf(stderr, "Invalid operands for %s\n", pl->mnemonic);
            fclose(outfile);
            goto cleanup;
        }

        // Encode instruction
        uint32_t code = encode_instruction(spec, rd, rs1, rs2, imm);
        fwrite(&code, 4, 1, outfile);
        free_parsed_line(pl);
    }

    fclose(outfile);
    printf("Assembled to a.out\n");
    free(lines);
    symbols_free(st);
    return 0;

cleanup:
    free(lines);
    symbols_free(st);
    return -1;
}
