#pragma once

#include <assert.h>

#include <complex>
#include <cstdint>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "../assembly/assembly.h"
#include "../include/file.h"
#include "../include/op.h"
#include "../include/program.h"

class Compiler {
  public:
    Compiler(Program program) : program(program) {}

    void generate_asm();
    void perform_optimizations();
    bool changes_made();
    void write_asm_to_file(std::string output_filename);  // TODO: impl
  private:
    void gen_op_asm(Op op, int func_addr);

    void handle_write_data_to_ptr(Op op);
    void handle_copy_ptr_to_ptr(Op op);
    void handle_read_data_from_ptr(Op op);

    void optimize_func();
    // stack optimizations
    void    handle_stack_pop_optim();
    void    handle_stack_push_optim();
    bool    register_is_used_before(Register reg, uint32_t end_idx);
    bool    register_is_used_before_being_set_again(Register reg, uint32_t start_idx);
    int64_t find_idx_for_top_push();

    // mov optimizations
    bool check_mov_reg_args(Instruction i);
    void handle_mov_reg_optim();
    void optimize_consecutive_movs(uint64_t idx);
    void compare_stored_values_mov(Instruction i, Register write_reg, int64_t save_value);

    bool arguments_equal(Argument a, Argument b);
    void erase_idx(uint32_t i);
    void set_idx(uint32_t i, Instruction inst);
    void remove_stored_register(Register reg);

    Program         program;
    AssemblyProgram asmp;

    std::map<Register, int64_t> stored_registers;
    uint32_t                       idx = 0;
    uint32_t                       number_of_changes = std::numeric_limits<uint32_t>::max();
    std::pair<std::string, std::vector<Instruction>> fn_key;
};
