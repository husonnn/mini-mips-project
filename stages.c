#include <stdio.h>

#include "stages.h"

// Get the next instruction
uint32_t fetch(state* st) {
    // TODO: Fetch the next instruction.

    // Fetches next instruction at the program counter's location in memory
    uint32_t next_instruction = 0;
    next_instruction |= st->memory[st->pc] << 24;
    next_instruction |= st->memory[st->pc+1] << 16;
    next_instruction |= st->memory[st->pc+2] <<  8;
    next_instruction |= st->memory[st->pc+3]      ;

    // Increments program counter
    st->pc += 4;

    // Returns next instruction
    return next_instruction;
}

// Decode the instruction
instruction decode(state* st, uint32_t instr) {
    // TODO: Convert the instruction to ALU inputs.
    
    // 
    instruction new_ins;
    uint32_t funct_r = instr & 0x0000003F;
    uint32_t op_code = ((instr & 0xFC000000) >> 1) & 0xCF000000;
    uint32_t rs_r = (instr & 0x03E00000) >> 21; // First Source Reg
    uint32_t rt_r = (instr & 0x001F0000) >> 16; // Second Source Register
    uint8_t rd_r = (instr & 0x0000F800) >> 11; // Destination Register (Format R)
    uint32_t shamt_r = (instr & 0x000007C0) >> 6; // Shift by
    uint32_t rd_i = (instr & 0x001F0000) >> 16; // Destination Register (Format i)
    int16_t imm = instr & 0x0000FFFF; // immediate argument
    uint32_t address = instr & 0x03FFFFFF;

    printf("%d\n", op_code);
    if(op_code == 0){ //if instruction is in R format
        switch(funct_r){
            case ADD:
                new_ins.left = (int32_t)st->regs[rs_r];
                new_ins.right = (int32_t)st->regs[rt_r];
                new_ins.dest = rd_r;
                new_ins.op = ADD;
                new_ins.ctrl = WB;
                break;
            case AND:
                new_ins.left = (int32_t)st->regs[rs_r];
                new_ins.right = (int32_t)st->regs[rt_r];
                new_ins.dest = rd_r;
                new_ins.op = AND;
                new_ins.ctrl = WB;
                break;
            case DIV:
                new_ins.left = (int32_t)st->regs[rs_r];
                new_ins.right = (int32_t)st->regs[rt_r];
                new_ins.dest = 0;
                new_ins.op = DIV;
                new_ins.ctrl = HILO;
                break;
            case EQ:
                // funct code for eq?
                new_ins.left = (int32_t)st->regs[rs_r];
                new_ins.right = (int32_t)st->regs[rt_r];
                new_ins.dest = rd_r;
                new_ins.op = EQ;
                new_ins.ctrl = WB;
                break;
            case LT:
                new_ins.left = (int32_t)st->regs[rs_r];
                new_ins.right = (int32_t)st->regs[rt_r];
                new_ins.dest = rd_r;
                new_ins.op = LT;
                new_ins.ctrl = WB;
                break;
            case MUL:
                new_ins.left = (int32_t)st->regs[rs_r];
                new_ins.right = (int32_t)st->regs[rt_r];
                new_ins.dest = 0;
                new_ins.op = MUL;
                new_ins.ctrl = HILO;
                break;
            case NEQ:
                // funct code for neq?
                new_ins.left = (int32_t)st->regs[rs_r];
                new_ins.right = (int32_t)st->regs[rt_r];
                new_ins.dest = rd_r;
                new_ins.op = NEQ;
                new_ins.ctrl = WB;
                break;
            case OR:
                new_ins.left = (int32_t)st->regs[rs_r];
                new_ins.right = (int32_t)st->regs[rt_r];
                new_ins.dest = rd_r;
                new_ins.op = OR;
                new_ins.ctrl = WB;
                break;
            case SLL:
                new_ins.left = 0;
                new_ins.right = (int32_t)st->regs[rt_r];
                new_ins.dest = (int32_t)st->regs[rd_r];
                new_ins.extra = (int32_t)st->regs[shamt_r];
                new_ins.op = SLL;
                new_ins.ctrl = WB;
                break;
            case SRA:
                new_ins.left = 0;
                new_ins.right = (int32_t)st->regs[rt_r];
                new_ins.dest = (int32_t)st->regs[rd_r];
                new_ins.extra = (int32_t)st->regs[shamt_r];
                new_ins.op = SRA;
                new_ins.ctrl = WB;
                break;
            case SRL:
                new_ins.left = 0;
                new_ins.right = (int32_t)st->regs[rt_r];
                new_ins.dest = (int32_t)st->regs[rd_r];
                new_ins.extra = (int32_t)st->regs[shamt_r];
                new_ins.op = SRL;
                new_ins.ctrl = WB;
                break;
            case SUB:
                new_ins.left = (int32_t)st->regs[rs_r];
                new_ins.right = (int32_t)st->regs[rt_r];
                new_ins.dest = rd_r;
                new_ins.op = SUB;
                new_ins.ctrl = WB;
                break;
            case XOR:
                new_ins.left = (int32_t)st->regs[rs_r];
                new_ins.right = (int32_t)st->regs[rt_r];
                new_ins.dest = rd_r;
                new_ins.op = XOR;
                new_ins.ctrl = WB;
                break;
        }
    }
    else{
        switch(op_code){ // if in j or I format
            case 0b10:
                // jump, j format
                new_ins.left = (int32_t)address << 2;
                new_ins.ctrl = JUMP;
                break;
            case 0b11:
                // jump and link, j format
                new_ins.left = (int32_t)address << 2;
                //where is return address???
                new_ins.ctrl = JAL;
                break;
            case 35:
                // lw
                new_ins.left = (int32_t)st->regs[rs_r];
                new_ins.dest = rd_i;
                new_ins.extra = (int32_t)imm;
                new_ins.ctrl = LW;
                break;
            case 0b101011:
                // sw
                new_ins.left = (int32_t)st->regs[rs_r];
                new_ins.dest = rd_i;
                new_ins.extra = (int32_t)imm;
                new_ins.ctrl = SW;
                break;
            default:
                new_ins.left = 0;
                new_ins.dest = 0;
                new_ins.ctrl = BRANCH;
        }
    }
    
   return new_ins;

}

/*
instruction struct

typedef struct {
  int32_t left;     // Left argument, 
  int32_t right;    // Right argument
  int32_t extra;    // Extra argument needed for some instructions
  uint8_t dest;     // Destination register index
  alu_op op;        // ALU operation
  control_op ctrl;  // Control operation
} instruction;
 */


// Execute the arithmetic and logical parts of the instruction
result execute(state* st, instruction instr) {
    // TODO: Call into the ALU and setup results as necessary.
    int32_t* hilo = NULL;
    result res = {0, 0, 0, NONE, 0};
    int32_t alu_res = alu(instr.left, instr.right, instr.op, hilo);
    
    // if instr.ctrl != 0 && instr.op != sll
    // then do ctrl operaton

    switch(instr.ctrl){
        case WB:
            res.value = alu_res;
            res.dest = instr.dest;
            res.op = WRITEBACK;
            break;
        case BRANCH:
            // not implemented
            res.value = instr.left;
            res.op = NONE;
            break;
        case HILO:
            // figure out later
            // not implemented
            res.hi_val = alu_res;
            res.dest = instr.dest;
            res.op = HILO_WB;
            break;
        case LW:
            // dest = rd_i
            // source = rs_r
            // imm = some integer
            //
            res.dest = instr.dest;
            res.value = instr.left + instr.extra;
            res.op = LOAD;
            break;
        case SW:
            res.dest = instr.dest; // destination
            res.value = instr.left + instr.extra; // rs + imm
            res.op = STORE;
            break;
        case JUMP:
            st->pc = instr.left;
            break;
        case JAL:
            res.value = st->pc +4;
            st->pc = instr.left;
            break;
    }
    // after all checks, check if mem or writeback.
    return res;
}

/*
int32_t alu(int32_t left, int32_t right, alu_op op, int32_t* hi)

typedef struct {
  int32_t value;   // Arithmetic/logical result
  int32_t hi_val;  // Extra result needed by some instructions
  uint8_t dest;    // Destination register index
  resop op;        // Memory/register operation for later stages
  uint8_t jump;    // boolean, true if we need to modify the PC
} result;
 */


// Perform any required memory updates.
result memory(state* st, result ex_res) {
    // TODO: Update memory if necessary
    // return 
    if(ex_res.op == STORE){
        //ex_res.value is value
        set_memory(st, ex_res.dest, ex_res.value);
    } else if(ex_res.op == LOAD){
        st->regs[ex_res.dest] = get_memory(st, ex_res.value);
    }
    return ex_res;
}



// Update any register values that need to be changed.
void writeback(state* st, result res) {
    // TODO: Update registers if necessary.

    // In every writeback command, store result value in the designated register.

    // regular wb

    if(res.op == WRITEBACK){
        st->regs[res.dest] = res.value;
    }else if(res.op == HILO_WB){
        st->regs[res.dest] = res.hi_val;
    }
}

void run_one_instruction(state* st) {
    // TODO: Run an instruction.
    uint32_t instr_uc = fetch(st);
    instruction instr_d = decode(st, instr_uc);
    result instr_ex = execute(st, instr_d);
    memory(st, instr_ex);
    writeback(st, instr_ex);
}

void run_program(state* st, size_t prog_len) {
  while (st->pc < 4 * prog_len) {
    run_one_instruction(st);
  }
}
