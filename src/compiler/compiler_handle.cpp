#include "compiler.h"

void Compiler::handle_write_data_to_ptr(Op op) {
    // rax is the ptr to write to
    // rbx is the data
    switch (op.mode) {
    case Op::Mode::BIT_8:
        asmp.new_inst(asmp.inst_mov(Argument(Register::RAX, true), Argument(Register::BL)));
        break;
    case Op::Mode::BIT_64:
        asmp.new_inst(asmp.inst_mov(Argument(Register::RAX, true), Argument(Register::RBX)));
        break;
    case Op::Mode::STRUCT: {
        asmp.new_inst(asmp.inst_mov(Argument(Register::RCX), Argument(Register::RBX, true)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RAX, true), Argument(Register::RCX)));
        for (int i = 8; i < op.int_operand_2; i += 8) {
            asmp.new_inst(
                asmp.inst_mov(Argument(Register::RCX), Argument(Register::RBX, true, INFER_NONE, i)));
            asmp.new_inst(
                asmp.inst_mov(Argument(Register::RAX, true, INFER_NONE, i), Argument(Register::RCX)));
        }
    } break;
    };
}

void Compiler::handle_copy_ptr_to_ptr(Op op) {
    // rax is ptr to write to
    // rbx is ptr to read from
    switch (op.mode) {
    case Op::Mode::BIT_8:
        asmp.new_inst(asmp.inst_mov(Argument(Register::CL), Argument(Register::RBX, true)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RAX, true), Argument(Register::CL)));
        break;
    case Op::Mode::BIT_64:
        asmp.new_inst(asmp.inst_mov(Argument(Register::RCX), Argument(Register::RBX, true)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RAX, true), Argument(Register::RCX)));
        break;
    case Op::Mode::STRUCT:
        print_error_at_loc(op.loc, "compiler error! this opcode (" +
                                       std::to_string(std::to_underlying(op.type)) +
                                       ") should not have Op::Mode::STRUCT passed into it. This is "
                                       "probably a bug in the parsing or type-checking stage");
        exit(1);
    }
}

void Compiler::handle_read_data_from_ptr(Op op) {
    // rax is the pointer to read from
    // rbx is the data pushed onto the stack
    // unless pushing struct; we then push rax onto stack
    switch (op.mode) {
    case Op::Mode::BIT_8:
        asmp.new_inst(asmp.inst_mov(Argument(Register::RBX), Argument(0)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::BL), Argument(Register::RAX, true)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RBX)));
        break;
    case Op::Mode::BIT_64:
        asmp.new_inst(asmp.inst_mov(Argument(Register::RBX), Argument(0)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RBX), Argument(Register::RAX, true)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RBX)));
        break;
    case Op::Mode::STRUCT: asmp.new_inst(asmp.inst_push(Argument(Register::RAX))); break;
    }
}
