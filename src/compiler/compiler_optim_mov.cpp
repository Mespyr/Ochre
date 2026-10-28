#include "compiler.h"

bool Compiler::check_mov_reg_args(Instruction i) {
    // returns false if there is an something that
    // makes it so the outside function couldn't optimize
    Argument write_reg = i.arguments.at(0);
    if (write_reg.read_pointer || write_reg.type != ARG_REGISTER) return false;
    Argument save_value = i.arguments.at(1);
    if (save_value.read_pointer) {
        remove_stored_register(write_reg.reg_value);
        return false;
    }
    return true;
}

void Compiler::compare_stored_values_mov(Instruction i, AsmRegister write_reg,
                                         int64_t save_value) {
    if (stored_registers.count(write_reg)) {
        int64_t diff = save_value - stored_registers.at(write_reg);
        if (diff == 0)
            erase_idx(idx);
        else if (diff < 0)
            fn_key.second.at(idx) =
                asmp.inst_sub(i.arguments.front(), Argument(-diff));
        else if (diff > 0)
            fn_key.second.at(idx) =
                asmp.inst_add(i.arguments.front(), Argument(diff));
        stored_registers[write_reg] = save_value;
        return;
    } else
        stored_registers.insert({write_reg, save_value});

    if (save_value == 0)
        fn_key.second.at(idx) =
            asmp.inst_xor(i.arguments.front(), i.arguments.front());
}

void Compiler::optimize_consecutive_movs(uint64_t mov_idx) {
    /*
      after we create a mov instruction after a push/pop, there is a small
      chance that this mov is right after another mov instruction. if the
      previous mov instruction sets the register we are currently using to set
      another, we can combine the two into one mov instruction, as long as the
      contents of the register in question aren't being used in a future
      instruction. We can check this by peering into future instructions and
      making sure the register is set to a new value before being read.

      ex:
        mov rax, 10
        mov rdx, rax
      converts to:
        mov rdx, 10
    */

    Instruction instruction = fn_key.second.at(mov_idx);
    Argument    set_register = instruction.arguments.at(0);
    Argument    read_register = instruction.arguments.at(1);

    if (read_register.type != ARG_REGISTER || read_register.read_pointer)
        return;

    Instruction previous_instruction = fn_key.second.at(mov_idx - 1);
    Argument previous_set_register = previous_instruction.arguments.at(0);

    if (previous_instruction.type != INSTRUCTION_MOV) return;
    if (previous_set_register.type != ARG_REGISTER || set_register.read_pointer)
        return;
    if (previous_set_register.reg_value != read_register.reg_value) return;

    if (register_is_used_before_being_set_again(read_register.reg_value,
                                                mov_idx))
        return;

    fn_key.second.at(mov_idx) =
        asmp.inst_mov(set_register, previous_instruction.arguments.at(1));
    erase_idx(mov_idx - 1);
}

void Compiler::handle_mov_reg_optim() {
    Instruction i = fn_key.second.at(idx);
    if (!check_mov_reg_args(i)) return;

    Argument write_reg = i.arguments.at(0);
    Argument save_value = i.arguments.at(1);
    int64_t  int_val;

    switch (save_value.type) {
    case ARG_INTEGER: int_val = save_value.int_value; break;

    case ARG_REGISTER: {
        if (!stored_registers.count(save_value.reg_value)) {
            remove_stored_register(write_reg.reg_value);
            return;
        }
        int_val = stored_registers.at(save_value.reg_value);
    } break;

    case ARG_VARIABLE: remove_stored_register(write_reg.reg_value); return;
    }

    if (stored_registers.count(write_reg.reg_value)) {
        int64_t diff = int_val - stored_registers.at(write_reg.reg_value);
        if (diff == 0) erase_idx(idx);
        stored_registers[write_reg.reg_value] = int_val;
    } else {
        stored_registers.insert({write_reg.reg_value, int_val});
        optimize_consecutive_movs(idx);
    }
}
