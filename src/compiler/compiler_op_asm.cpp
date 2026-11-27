#include "compiler.h"

void Compiler::gen_op_asm(Op op, int func_addr) {
    switch (op.type) {
    case Op::Type::DUMP: {
        asmp.compile_dump = true;
        asmp.new_inst(asmp.inst_pop(Argument(Register::RDI)));
        asmp.new_inst(asmp.inst_call("dump"));
    } break;

    case Op::Type::PLUS: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_add(Argument(Register::RBX), Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RBX)));
    } break;

    case Op::Type::MINUS: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_sub(Argument(Register::RBX), Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RBX)));
    } break;

    case Op::Type::MUL: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_imul(Argument(Register::RAX), Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
    } break;

    case Op::Type::DIV: {
        asmp.new_inst(asmp.inst_mov(Argument(Register::RDX), Argument(0)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_div(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RDX)));
    } break;

    case Op::Type::EQUAL: {
        asmp.new_inst(asmp.inst_mov(Argument(Register::RCX), Argument(0)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RDX), Argument(1)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_cmp(Argument(Register::RAX), Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_cmove(Argument(Register::RCX), Argument(Register::RDX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RCX)));
    } break;

    case Op::Type::GREATER: {
        asmp.new_inst(asmp.inst_mov(Argument(Register::RCX), Argument(0)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RDX), Argument(1)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_cmp(Argument(Register::RAX), Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_cmovg(Argument(Register::RCX), Argument(Register::RDX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RCX)));
    } break;

    case Op::Type::LESS: {
        asmp.new_inst(asmp.inst_mov(Argument(Register::RCX), Argument(0)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RDX), Argument(1)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_cmp(Argument(Register::RAX), Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_cmovl(Argument(Register::RCX), Argument(Register::RDX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RCX)));
    } break;

    case Op::Type::GREATER_EQ: {
        asmp.new_inst(asmp.inst_mov(Argument(Register::RCX), Argument(0)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RDX), Argument(1)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_cmp(Argument(Register::RAX), Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_cmovge(Argument(Register::RCX), Argument(Register::RDX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RCX)));
    } break;

    case Op::Type::LESS_EQ: {
        asmp.new_inst(asmp.inst_mov(Argument(Register::RCX), Argument(0)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RDX), Argument(1)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_cmp(Argument(Register::RAX), Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_cmovle(Argument(Register::RCX), Argument(Register::RDX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RCX)));
    } break;

    case Op::Type::NOT_EQ: {
        asmp.new_inst(asmp.inst_mov(Argument(Register::RCX), Argument(0)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RDX), Argument(1)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_cmp(Argument(Register::RAX), Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_cmovne(Argument(Register::RCX), Argument(Register::RDX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RCX)));
    } break;

    case Op::Type::AND: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_and(Argument(Register::RBX), Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RBX)));
    } break;

    case Op::Type::OR: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_or(Argument(Register::RBX), Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RBX)));
    } break;

    case Op::Type::NOT: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_test(Argument(Register::RAX), Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_sete(Argument(Register::AL)));
        asmp.new_inst(asmp.inst_movzx(Argument(Register::RAX), Argument(Register::AL)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
    } break;

    case Op::Type::POP: asmp.new_inst(asmp.inst_pop(Argument(Register::RAX))); break;

    case Op::Type::DUP: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
        // asmp.new_inst(asmp.inst_push(Argument(Register::RSP, true,
        // INFER_QWORD)));
    } break;

    case Op::Type::SWP: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RBX)));
    } break;

    case Op::Type::ROT: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RCX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RCX)));
    } break;

    case Op::Type::OVER: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RBX)));
    } break;

    case Op::Type::SET_VAR:
    case Op::Type::SET_VAR_STRUCT_MEMBER: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RAX), Argument("ret_stack_rsp", true)));
        asmp.new_inst(asmp.inst_add(Argument(Register::RAX), Argument(op.int_operand)));
        handle_write_data_to_ptr(op);
    } break;

    case Op::Type::SET_VAR_FROM_OTHER_PTR: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RAX), Argument("ret_stack_rsp", true)));
        asmp.new_inst(asmp.inst_add(Argument(Register::RAX), Argument(op.int_operand)));
        handle_copy_ptr_to_ptr(op);
    } break;

    case Op::Type::READ_VAR:
    case Op::Type::READ_VAR_STRUCT_MEMBER: {
        asmp.new_inst(asmp.inst_mov(Argument(Register::RAX), Argument("ret_stack_rsp", true)));
        asmp.new_inst(asmp.inst_add(Argument(Register::RAX), Argument(op.int_operand)));
        handle_read_data_from_ptr(op);
    } break;

    case Op::Type::PUSH_VAR: {
        asmp.new_inst(asmp.inst_mov(Argument(Register::RAX), Argument("ret_stack_rsp", true)));
        asmp.new_inst(asmp.inst_add(Argument(Register::RAX), Argument(op.int_operand)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
    } break;

    case Op::Type::SET_PTR: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        handle_write_data_to_ptr(op);
    } break;

    case Op::Type::SET_PTR_FROM_OTHER_PTR: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        handle_copy_ptr_to_ptr(op);
    } break;

    case Op::Type::SET_PTR_STRUCT_MEMBER: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_add(Argument(Register::RAX), Argument(op.int_operand)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RBX)));
        handle_write_data_to_ptr(op);
    } break;

    case Op::Type::READ_PTR: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RBX), Argument(0)));
        handle_read_data_from_ptr(op);
    } break;

    case Op::Type::READ_PTR_STRUCT_MEMBER: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_add(Argument(Register::RAX), Argument(op.int_operand)));
        handle_read_data_from_ptr(op);
    } break;

    case Op::Type::SYSCALL0: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_syscall());
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
    } break;
    case Op::Type::SYSCALL1: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RDI)));
        asmp.new_inst(asmp.inst_syscall());
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
    } break;
    case Op::Type::SYSCALL2: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RDI)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RSI)));
        asmp.new_inst(asmp.inst_syscall());
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
    } break;
    case Op::Type::SYSCALL3: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RDI)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RSI)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RDX)));
        asmp.new_inst(asmp.inst_syscall());
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
    } break;
    case Op::Type::SYSCALL4: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RDI)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RSI)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RDX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::R10)));
        asmp.new_inst(asmp.inst_syscall());
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
    } break;
    case Op::Type::SYSCALL5: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RDI)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RSI)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RDX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::R10)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::R8)));
        asmp.new_inst(asmp.inst_syscall());
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
    } break;
    case Op::Type::SYSCALL6: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RDI)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RSI)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RDX)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::R10)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::R8)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::R9)));
        asmp.new_inst(asmp.inst_syscall());
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
    } break;

    case Op::Type::LABEL:
        asmp.new_inst(asmp.inst_label("addr_" + std::to_string(func_addr) + "_" +
                                      std::to_string(op.int_operand)));
        break;

    case Op::Type::JMP:
        asmp.new_inst(asmp.inst_jmp("addr_" + std::to_string(func_addr) + "_" +
                                    std::to_string(op.int_operand)));
        break;

    case Op::Type::CJMPT: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_test(Argument(Register::RAX), Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_jnz("addr_" + std::to_string(func_addr) + "_" +
                                    std::to_string(op.int_operand)));
    } break;

    case Op::Type::CJMPF: {
        asmp.new_inst(asmp.inst_pop(Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_test(Argument(Register::RAX), Argument(Register::RAX)));
        asmp.new_inst(asmp.inst_jz("addr_" + std::to_string(func_addr) + "_" +
                                   std::to_string(op.int_operand)));
    } break;

    case Op::Type::PUSH_INT: {
        asmp.new_inst(asmp.inst_mov(Argument(Register::RAX), Argument(op.int_operand)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
    } break;

    case Op::Type::PUSH_CHAR: {
        asmp.new_inst(asmp.inst_mov(Argument(Register::RAX), Argument(op.int_operand)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
    } break;

    case Op::Type::PUSH_STR: {
        asmp.string_data.push_back(op.str_operand);
        asmp.new_inst(asmp.inst_mov(Argument(Register::RAX), Argument(op.str_operand.length())));
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
        asmp.new_inst(
            asmp.inst_push(Argument("str_" + std::to_string(asmp.string_data.size() - 1))));
    } break;

    case Op::Type::PUSH_ARGV: {
        asmp.new_inst(asmp.inst_mov(Argument(Register::RAX), Argument("args_ptr", true)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RBX), Argument(Register::RAX, true)));
        asmp.new_inst(asmp.inst_add(Argument(Register::RAX), Argument(8)));
        asmp.new_inst(asmp.inst_push(Argument(Register::RBX)));  // the length of argv
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));  // argv itself
    } break;

    case Op::Type::PUSH_TYPE_INSTANCE: {
        asmp.new_inst(asmp.inst_mov(Argument(Register::RAX), Argument(9)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RDI), Argument(0)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RSI), Argument(op.int_operand)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RDX), Argument(3)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::R10), Argument(33)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::R8), Argument(-1)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::R9), Argument(0)));
        asmp.new_inst(asmp.inst_syscall());
        asmp.new_inst(asmp.inst_push(Argument(Register::RAX)));
    } break;

    case Op::Type::DELETE_PTR: {
        asmp.new_inst(asmp.inst_mov(Argument(Register::RAX), Argument(11)));
        asmp.new_inst(asmp.inst_pop(Argument(Register::RDI)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RSI), Argument(op.int_operand)));
        asmp.new_inst(asmp.inst_syscall());
    } break;

    case Op::Type::TYPE_CAST: break;  // nothing needed to do!

    case Op::Type::FUNCTION_CALL: {
        asmp.new_inst(asmp.inst_mov(Argument(Register::RAX), Argument(Register::RSP)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RSP), Argument("ret_stack_rsp", true)));
        asmp.new_inst(asmp.inst_call("func_addr_" +
                                     std::to_string(program.functions.at(op.str_operand).addr)));
        asmp.new_inst(asmp.inst_mov(Argument("ret_stack_rsp", true), Argument(Register::RSP)));
        asmp.new_inst(asmp.inst_mov(Argument(Register::RSP), Argument(Register::RAX)));
    } break;

    case Op::Type::RETURN:
        asmp.new_inst(asmp.inst_jmp("addr_" + std::to_string(func_addr) + "_return"));
        break;

    case Op::Type::CONST:
    case Op::Type::DEFINE_VAR:
    case Op::Type::FUN:
    case Op::Type::IMPORT:
    case Op::Type::READ:
    case Op::Type::SET:
    case Op::Type::STRUCT:
    case Op::Type::COUNT:
        print_error_at_loc(op.loc, "compiler error! this opcode (" +
                                       std::to_string(std::to_underlying(op.type)) +
                                       ") shouldn't be passed into compiling stage");
        exit(1);
    }
}
