CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -Isrc -g -O0

SRC=src/main.c src/assembler.c src/parser.c src/registers.c \
    src/instr_table.c src/symbols.c src/encode.c

TESTS=tests/test_registers tests/test_parser tests/test_instr_table \
      tests/test_symbols tests/test_encode tests/test_assembler

ASSEMBLER=assembler

all: $(ASSEMBLER)

$(ASSEMBLER): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $@

# Test rules
tests/test_%: tests/test_%.c src/%.c src/assembler.c src/parser.c src/registers.c \
               src/instr_table.c src/symbols.c src/encode.c
	$(CC) $(CFLAGS) $^ -o $@

test: $(TESTS)
	@for test in $(TESTS); do \
		echo "Running $$test..."; \
		./$$test && echo "$$test PASSED" || exit 1; \
	done
	@echo "All tests passed!"

run: $(ASSEMBLER)
	@echo "To test: ./assembler examples/test_case_1.s"

clean:
	rm -f $(ASSEMBLER) $(TESTS) a.out test.s *.o

.PHONY: all test run clean
