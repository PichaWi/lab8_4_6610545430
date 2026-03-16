# RISC-V Assembler Lab (219223 Computer System Laboratory Lab 8-4)

## Project Overview
This repository implements a RV32I two-pass assembler in C. It parses simple RISC-V assembly, builds a symbol table, resolves labels, and writes encoded machine words to an output binary file.

## Files and Modules
- `src/main.c` — Entry point CLI; calls `assemble_file()`
- `src/assembler.h` — Assembler API declaration
- `src/assembler.c` — Two-pass assembler driver, symbol table, instruction encoding loop
- `src/parser.h` — Parse data structures and APIs
- `src/parser.c` — Line tokenizer, label/mnemonic/operand extraction
- `src/registers.h` — Register parser API
- `src/registers.c` — Convert `x0..x31` to register numbers
- `src/instr_table.h` — Instruction table declarations and format enum
- `src/instr_table.c` — RV32I opcode/funct3/funct7 table and lookup
- `src/symbols.h` — Symbol table interface
- `src/symbols.c` — Symbol add/find, dynamic storage
- `src/encode.h` — Instruction encoder API
- `src/encode.c` — Encode R/I/S/B/J binary format

## Tests
Unit tests are in `tests/`:
- `tests/test_registers.c`
- `tests/test_parser.c`
- `tests/test_instr_table.c`
- `tests/test_symbols.c`
- `tests/test_encode.c`
- `tests/test_assembler.c`

## Features
- Two-pass architecture (symbol table pass + encode pass)
- Label-only lines and label+instruction lines
- Supports instruction formats: R, I, S, B, J
- Numeric immediates and label branch offsets
- Basic syntax and operand validation
- CLI output binary `a.out` by default

## Build & Run (Windows/MSYS2)
1. Build:
   ```bash
   gcc -Wall -Wextra -I. -Isrc src/main.c src/assembler.c src/parser.c src/registers.c src/instr_table.c src/symbols.c src/encode.c -o assembler
   ```
   Or use `make` if available.

2. Run assembler:
   ```bash
   ./assembler input.s
   ```
   This writes 
   - `a.out` (binary output) or
   - can be adapted to text hex output by modifying source.

3. Run tests:
   ```bash
   gcc -Wall -Wextra -I. -Isrc tests/test_registers.c src/registers.c -o test_registers
   ./test_registers
   ```
   Or run all tests with `make test`.

## Example `sample.s`
```s
start: addi x1, x0, 5
       add x2, x1, x1
       beq x2, x0, start
```
Run:
```bash
./assembler sample.s
```

## Notes
- For branch/jump label resolution, assembler computes PC-relative offsets.
- The codebase is educational; extend by adding more instructions (U/A instructions) or improved parsing and output formatting.

## Quick Troubleshooting
- If `assembler` prints usage, run with `assembler <file.s>`
- If encoding fails, check tokens and operand counts in assembly lines.

---
Happy assembling! 🧠
