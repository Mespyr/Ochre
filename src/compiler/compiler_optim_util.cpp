#include "compiler.h"

bool Compiler::arguments_equal(Argument a, Argument b) { return a.to_string() == b.to_string(); }

bool Compiler::changes_made() { return number_of_changes > 0; }

void Compiler::erase_idx(uint32_t i) {
    fn_key.second.erase(fn_key.second.begin() + i);
    idx--;
    number_of_changes++;
}

void Compiler::set_idx(uint32_t i, Instruction inst) {
    fn_key.second.at(i) = inst;
    number_of_changes++;
}

void Compiler::remove_stored_register(Register reg) {
    for (auto it = stored_registers.begin(); it != stored_registers.end();)
        if (it->first == reg)
            it = stored_registers.erase(it);
        else
            ++it;
}

bool Compiler::register_is_used_before(Register reg, uint32_t end_idx) {
    for (uint32_t i = idx - 1; i > end_idx; i--) {
        Instruction inst = fn_key.second.at(i);
        switch (inst.type) {
        // 2 arity
        case Instruction::Type::ADD:
        case Instruction::Type::SUB:
        case Instruction::Type::IMUL:
        case Instruction::Type::MOV:
        case Instruction::Type::MOVZX:
        case Instruction::Type::CMP:
        case Instruction::Type::CMOVE:
        case Instruction::Type::CMOVG:
        case Instruction::Type::CMOVL:
        case Instruction::Type::CMOVGE:
        case Instruction::Type::CMOVLE:
        case Instruction::Type::CMOVNE:
        case Instruction::Type::AND:
        case Instruction::Type::OR: {
            Argument a = inst.arguments.front();
            Argument b = inst.arguments.back();
            if ((a.type == Argument::Type::REGISTER && a.reg_value == reg) ||
                (b.type == Argument::Type::REGISTER && b.reg_value == reg))
                return true;
        } break;

        // 1 arity
        case Instruction::Type::INC:
        case Instruction::Type::DEC:
        case Instruction::Type::PUSH:
        case Instruction::Type::POP:
        case Instruction::Type::SETE: {
            Argument a = inst.arguments.front();
            if (a.type == Argument::Type::REGISTER && a.reg_value == reg) return true;
        } break;

        /* only one that really modifies more than js the register specified
           without using another op before or after it
         */
        case Instruction::Type::DIV: {
            Argument a = inst.arguments.front();
            if ((a.type == Argument::Type::REGISTER && a.reg_value == reg) ||
                (reg == Register::RAX) || (reg == Register::RDX))
                return true;
        } break;

        default: break;
        }
    }
    return false;
}

bool Compiler::register_is_used_before_being_set_again(Register reg, uint32_t start_idx) {
    // true -> register value was used
    // false -> register was set again without being used
    for (uint32_t i = start_idx + 1; i < fn_key.second.size(); i++) {
        Instruction inst = fn_key.second.at(i);
        switch (inst.type) {
        // special
        case Instruction::Type::POP: {
            Argument arg = inst.arguments.front();
            if (arg.type != Argument::Type::REGISTER || arg.read_pointer) break;
            if (arg.reg_value == reg) return false;
        } break;

        case Instruction::Type::PUSH: {
            Argument arg = inst.arguments.front();
            if (arg.type != Argument::Type::REGISTER) break;
            if (arg.reg_value == reg) return true;
        } break;

        case Instruction::Type::MOV:
        case Instruction::Type::MOVZX: {
            Argument arg1 = inst.arguments.front();
            Argument arg2 = inst.arguments.back();
            if (arg1.type == Argument::Type::REGISTER) {
                if (arg1.reg_value == reg) {
                    // u are using the value inside the register to write data
                    if (arg1.read_pointer)
                        return true;
                    else
                        return false;
                }
            }
            if (arg2.type == Argument::Type::REGISTER && arg2.reg_value == reg) return true;
        } break;
        case Instruction::Type::DIV: {
            // div uses rax, rbx and rdx
            // rbx is not changed, but rax and rdx are
            Argument arg = inst.arguments.front();
            if (arg.type == Argument::Type::REGISTER && arg.reg_value == reg) return true;  // rbx
            if (reg == Register::RAX || reg == Register::RDX) return false;
        } break;

        // 2-arity using both arguments
        case Instruction::Type::ADD:
        case Instruction::Type::SUB:
        case Instruction::Type::IMUL:
        case Instruction::Type::CMP:
        case Instruction::Type::CMOVE:
        case Instruction::Type::CMOVG:
        case Instruction::Type::CMOVL:
        case Instruction::Type::CMOVGE:
        case Instruction::Type::CMOVLE:
        case Instruction::Type::CMOVNE: {
            Argument arg1 = inst.arguments.front();
            Argument arg2 = inst.arguments.back();
            if ((arg1.type == Argument::Type::REGISTER && arg1.reg_value == reg) ||
                (arg2.type == Argument::Type::REGISTER && arg2.reg_value == reg))
                return true;
        } break;

        case Instruction::Type::CALL:
        case Instruction::Type::SYSCALL:
        case Instruction::Type::LABEL:
        case Instruction::Type::JMP:
        case Instruction::Type::JZ:
        case Instruction::Type::JNZ:
        case Instruction::Type::RET:  // JUST TO MAKE SURE IDK
            return true;

        default: break;
        }
    }
    return false;
}

int64_t Compiler::find_idx_for_top_push() {
    int32_t indent = 0;
    for (uint32_t i = idx - 1; i > 0; i--) {
        Instruction inst = fn_key.second.at(i);
        switch (inst.type) {
        case Instruction::Type::POP: indent++; break;

        case Instruction::Type::PUSH: {
            if (indent > 0)
                indent--;
            else
                return i;
        } break;

        case Instruction::Type::CALL:
        case Instruction::Type::LABEL:
        case Instruction::Type::JMP:
        case Instruction::Type::JZ:
        case Instruction::Type::JNZ:
        case Instruction::Type::SYSCALL:
        case Instruction::Type::RET:  // JUST TO MAKE SURE IDK
            return -1;

        default: break;
        }
    }

    return -1;
}
