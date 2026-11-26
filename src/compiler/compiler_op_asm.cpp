#include <error.h>

#include <cstdlib>

#include "compiler.h"

void Compiler::gen_op_asm(Op op, int func_addr) {
    switch (op.type) {
    case Op::Type::DUMP: {
        asmp.compile_dump = true;
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDI)));
        asmp.new_inst(asmp.inst_call("dump"));
    } break;

    case Op::Type::PLUS: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_add(Argument(REGISTER_RBX), Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));
    } break;

    case Op::Type::MINUS: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_sub(Argument(REGISTER_RBX), Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));
    } break;

    case Op::Type::MUL: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_imul(Argument(REGISTER_RAX), Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
    } break;

    case Op::Type::DIV: {
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDX), Argument(0)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_div(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RDX)));
    } break;

    case Op::Type::EQUAL: {
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RCX), Argument(0)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDX), Argument(1)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_cmp(Argument(REGISTER_RAX), Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_cmove(Argument(REGISTER_RCX), Argument(REGISTER_RDX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RCX)));
    } break;

    case Op::Type::GREATER: {
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RCX), Argument(0)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDX), Argument(1)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_cmp(Argument(REGISTER_RAX), Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_cmovg(Argument(REGISTER_RCX), Argument(REGISTER_RDX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RCX)));
    } break;

    case Op::Type::LESS: {
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RCX), Argument(0)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDX), Argument(1)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_cmp(Argument(REGISTER_RAX), Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_cmovl(Argument(REGISTER_RCX), Argument(REGISTER_RDX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RCX)));
    } break;

    case Op::Type::GREATER_EQ: {
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RCX), Argument(0)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDX), Argument(1)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_cmp(Argument(REGISTER_RAX), Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_cmovge(Argument(REGISTER_RCX), Argument(REGISTER_RDX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RCX)));
    } break;

    case Op::Type::LESS_EQ: {
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RCX), Argument(0)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDX), Argument(1)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_cmp(Argument(REGISTER_RAX), Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_cmovle(Argument(REGISTER_RCX), Argument(REGISTER_RDX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RCX)));
    } break;

    case Op::Type::NOT_EQ: {
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RCX), Argument(0)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDX), Argument(1)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_cmp(Argument(REGISTER_RAX), Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_cmovne(Argument(REGISTER_RCX), Argument(REGISTER_RDX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RCX)));
    } break;

    case Op::Type::AND: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_and(Argument(REGISTER_RBX), Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));
    } break;

    case Op::Type::OR: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_or(Argument(REGISTER_RBX), Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));
    } break;

    case Op::Type::NOT: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_test(Argument(REGISTER_RAX), Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_sete(Argument(REGISTER_AL)));
        asmp.new_inst(asmp.inst_movzx(Argument(REGISTER_RAX), Argument(REGISTER_AL)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
    } break;

    case Op::Type::POP: asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX))); break;

    case Op::Type::DUP: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
        // asmp.new_inst(asmp.inst_push(Argument(REGISTER_RSP, true,
        // INFER_QWORD)));
    } break;

    case Op::Type::SWP: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));
    } break;

    case Op::Type::ROT: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RCX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RCX)));
    } break;

    case Op::Type::OVER: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));
    } break;

    case Op::Type::SET_VAR:
    case Op::Type::SET_VAR_STRUCT_MEMBER: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument("ret_stack_rsp", true)));
        asmp.new_inst(asmp.inst_add(Argument(REGISTER_RAX), Argument(op.int_operand)));
        handle_write_data_to_ptr(op);
    } break;

    case Op::Type::SET_VAR_FROM_OTHER_PTR: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument("ret_stack_rsp", true)));
        asmp.new_inst(asmp.inst_add(Argument(REGISTER_RAX), Argument(op.int_operand)));
        handle_copy_ptr_to_ptr(op);
    } break;

    case Op::Type::READ_VAR:
    case Op::Type::READ_VAR_STRUCT_MEMBER: {
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument("ret_stack_rsp", true)));
        asmp.new_inst(asmp.inst_add(Argument(REGISTER_RAX), Argument(op.int_operand)));
        handle_read_data_from_ptr(op);
    } break;

    case Op::Type::PUSH_VAR: {
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument("ret_stack_rsp", true)));
        asmp.new_inst(asmp.inst_add(Argument(REGISTER_RAX), Argument(op.int_operand)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
    } break;

    case Op::Type::SET_PTR: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        handle_write_data_to_ptr(op);
    } break;

    case Op::Type::SET_PTR_FROM_OTHER_PTR: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        handle_copy_ptr_to_ptr(op);
    } break;

    case Op::Type::SET_PTR_STRUCT_MEMBER: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_add(Argument(REGISTER_RAX), Argument(op.int_operand)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RBX)));
        handle_write_data_to_ptr(op);
    } break;

    case Op::Type::READ_PTR: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RBX), Argument(0)));
        handle_read_data_from_ptr(op);
    } break;

    case Op::Type::READ_PTR_STRUCT_MEMBER: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_add(Argument(REGISTER_RAX), Argument(op.int_operand)));
        handle_read_data_from_ptr(op);
    } break;

    case Op::Type::SYSCALL0: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_syscall());
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
    } break;
    case Op::Type::SYSCALL1: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDI)));
        asmp.new_inst(asmp.inst_syscall());
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
    } break;
    case Op::Type::SYSCALL2: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDI)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RSI)));
        asmp.new_inst(asmp.inst_syscall());
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
    } break;
    case Op::Type::SYSCALL3: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDI)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RSI)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDX)));
        asmp.new_inst(asmp.inst_syscall());
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
    } break;
    case Op::Type::SYSCALL4: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDI)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RSI)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_R10)));
        asmp.new_inst(asmp.inst_syscall());
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
    } break;
    case Op::Type::SYSCALL5: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDI)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RSI)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_R10)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_R8)));
        asmp.new_inst(asmp.inst_syscall());
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
    } break;
    case Op::Type::SYSCALL6: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDI)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RSI)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDX)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_R10)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_R8)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_R9)));
        asmp.new_inst(asmp.inst_syscall());
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
    } break;

    case Op::Type::LABEL:
    case Op::Type::LABEL_END:
        asmp.new_inst(asmp.inst_label("addr_" + std::to_string(func_addr) + "_" +
                                      std::to_string(op.int_operand)));
        break;

    case Op::Type::JMP:
    case Op::Type::JMPE:
        asmp.new_inst(asmp.inst_jmp("addr_" + std::to_string(func_addr) + "_" +
                                    std::to_string(op.int_operand)));
        break;

    case Op::Type::CJMPT:
    case Op::Type::CJMPET: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_test(Argument(REGISTER_RAX), Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_jnz("addr_" + std::to_string(func_addr) + "_" +
                                    std::to_string(op.int_operand)));
    } break;

    case Op::Type::CJMPF:
    case Op::Type::CJMPEF: {
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_test(Argument(REGISTER_RAX), Argument(REGISTER_RAX)));
        asmp.new_inst(asmp.inst_jz("addr_" + std::to_string(func_addr) + "_" +
                                   std::to_string(op.int_operand)));
    } break;

    case Op::Type::PUSH_INT: {
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument(op.int_operand)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
    } break;

    case Op::Type::PUSH_CHAR: {
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument(op.int_operand)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
    } break;

    case Op::Type::PUSH_STR: {
        asmp.string_data.push_back(op.str_operand);
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument(op.str_operand.length())));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
        asmp.new_inst(
            asmp.inst_push(Argument("str_" + std::to_string(asmp.string_data.size() - 1))));
    } break;

    case Op::Type::PUSH_ARGV: {
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument("args_ptr", true)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RBX), Argument(REGISTER_RAX, true)));
        asmp.new_inst(asmp.inst_add(Argument(REGISTER_RAX), Argument(8)));
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RBX)));  // the length of argv
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));  // argv itself
    } break;

    case Op::Type::PUSH_TYPE_INSTANCE: {
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument(9)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDI), Argument(0)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RSI), Argument(op.int_operand)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RDX), Argument(3)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_R10), Argument(33)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_R8), Argument(-1)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_R9), Argument(0)));
        asmp.new_inst(asmp.inst_syscall());
        asmp.new_inst(asmp.inst_push(Argument(REGISTER_RAX)));
    } break;

    case Op::Type::DELETE_PTR: {
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument(11)));
        asmp.new_inst(asmp.inst_pop(Argument(REGISTER_RDI)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RSI), Argument(op.int_operand)));
        asmp.new_inst(asmp.inst_syscall());
    } break;

    case Op::Type::TYPE_CAST: break;  // nothing needed to do!

    case Op::Type::FUNCTION_CALL: {
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RAX), Argument(REGISTER_RSP)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RSP), Argument("ret_stack_rsp", true)));
        asmp.new_inst(asmp.inst_call("func_addr_" +
                                     std::to_string(program.functions.at(op.str_operand).addr)));
        asmp.new_inst(asmp.inst_mov(Argument("ret_stack_rsp", true), Argument(REGISTER_RSP)));
        asmp.new_inst(asmp.inst_mov(Argument(REGISTER_RSP), Argument(REGISTER_RAX)));
    } break;

    case Op::Type::RETURN:
        asmp.new_inst(asmp.inst_jmp("addr_" + std::to_string(func_addr) + "_return"));
        break;

    case Op::Type::CONST:
    case Op::Type::DEFINE_VAR:
    case Op::Type::END:
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
