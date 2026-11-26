#include "compiler.h"

void Compiler::optimize_func() {
    idx = 0;
    stored_registers.clear();
    while (idx < fn_key.second.size()) {
        Instruction i = fn_key.second.at(idx);

        switch (i.type) {
        case Instruction::Type::ADD:
        case Instruction::Type::SUB: {
            if (i.arguments.at(1).type == Argument::Type::INTEGER &&
                i.arguments.at(1).int_value == 0)
                erase_idx(idx);
            else if (i.arguments.front().type == Argument::Type::REGISTER &&
                     i.arguments.front().read_pointer == false)
                remove_stored_register(i.arguments.front().reg_value);
        } break;

        case Instruction::Type::MOV: handle_mov_reg_optim(); break;
        case Instruction::Type::POP: handle_stack_pop_optim(); break;
        case Instruction::Type::PUSH: handle_stack_push_optim(); break;

        case Instruction::Type::CALL:
        case Instruction::Type::SYSCALL:
        case Instruction::Type::LABEL:
        case Instruction::Type::JMP:
        case Instruction::Type::JZ:
        case Instruction::Type::JNZ:
            // cant be tracked across functions or jumps
            stored_registers.clear();
            break;

        case Instruction::Type::CMOVE:
        case Instruction::Type::CMOVG:
        case Instruction::Type::CMOVL:
        case Instruction::Type::CMOVGE:
        case Instruction::Type::CMOVLE:
        case Instruction::Type::CMOVNE:
            // involved arguments cannot be tracked
            remove_stored_register(i.arguments.front().reg_value);
            remove_stored_register(i.arguments.back().reg_value);
            break;

        default: break;
        }
        idx++;
    }
}

void Compiler::perform_optimizations() {
    number_of_changes = 0;  // reset number of changes for current pass
    for (auto key : asmp.code) {
        fn_key = key;
        optimize_func();
        asmp.code.at(fn_key.first) = fn_key.second;
    }
}
