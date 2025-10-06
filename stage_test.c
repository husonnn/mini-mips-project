#include <stdio.h>

#include "base.h"
#include "stages.h"
#include "instructions.h"
#include "assembler.h"

// Given an assembly program, write it to memory and then run it.
void run_case(state* st, asm_line* lines, size_t len) {
  size_t out_len;
  asm_prog p = { .lines = lines, .length = len };
  uint32_t* prog = assemble(p, 0, &out_len);
  write_program(st, 0, prog, out_len);
  run_program(st, out_len);
  free(prog);
}

// Run an assembly program and ensure that the given register has the expected
// value afterwards. This function also checks that no other register values
// have changed. Returns 1 if the test passes and 0 if it fails.
int register_test(asm_line* lines, size_t len, uint8_t reg, int32_t expected) {
  int64_t initial[32];
  state st;
  for (int i = 0; i < 32; i++) {
    if (i % 2 == 0) {
      st.regs[i] = i;
    } else {
      st.regs[i] = -i;
    }
    initial[i] = st.regs[i];
  }
  st.hi_reg = 0;
  st.lo_reg = 0;
  st.pc = 0;
  st.memory = malloc(100 * sizeof(uint8_t));

  run_case(&st, lines, len);
  int res = 1;
  for (int i = 0; i < 32; i++) {
    if (i == reg) {
      if (st.regs[reg] != expected) {
        printf("Expected %d, got %d\n", expected, st.regs[reg]);
        res = 0;
      }
    } else {
      if (st.regs[i] != initial[i]) {
        res = 0;
      }
    }
  }

  free(st.memory);

  return res;
}

// Test that after running the given assembly program, the memory at the given
// address has the expected value. Returns 1 if the test passes and 0 if it
// fails.
int memory_test(asm_line* lines, size_t len, uint32_t addr, int32_t expected) {
  state st;
  for (int i = 0; i < 32; i++) {
    if (i % 2 == 0) {
      st.regs[i] = i;
    } else {
      st.regs[i] = -i;
    }
  }
  st.hi_reg = 0;
  st.lo_reg = 0;
  st.pc = 0;
  st.memory = malloc(100 * sizeof(uint8_t));

  run_case(&st, lines, len);

  int32_t mem_val = get_memory(&st, addr);
  int res = (mem_val == expected);
  if (!res) {
    printf("Expected %d, got %d\n", expected, mem_val);
  }

  free(st.memory);

  return res;

}

// Report a test result. The first input is expected to be the output of either
// register_test or memory_test. The message is a short identifier for the test
// that was run. Failures and total are modified to reflect the total number of
// tests that have been run and the total number of test failures.
void run_test(int succeeded, char* msg, int* failures, int* total) {
  if (!succeeded) {
    printf("Failed %s test\n", msg);
    (*failures)++;
  }
  (*total)++;
}

int main() {

  int failures = 0;
  int total = 0;
  /*
  asm_line beq_lines[] = {
    BEQ(2, 2, "branch"),
    ADDI(6, 0, 1),
    LABEL("branch"),
    ADDI(6, 6, 1)
  };
  run_test(register_test(beq_lines, 4, 6, 7), "beq", &failures, &total);

  printf("Failures: %d / %d\n", failures, total);
  */
  
  asm_line beq_lines[] = {
      SW(1, 3, 0),
      SW(3, 1, 0)
  };
  run_test(register_test(beq_lines, 2, 1, 9999), "jump", &failures, &total);

  printf("Failures: %d / %d\n", failures, total);
}
