#include <cstdint>

#include "assembly.h"

std::string Argument::to_string() {
    std::string output;

    switch (size_inference) {
    case INFER_BYTE: output.append("BYTE "); break;
    case INFER_QWORD: output.append("QWORD "); break;
    case INFER_NONE: break;
    }

    if (read_pointer) output.append("[");

    switch (type) {
    case Argument::Type::INTEGER: output.append(std::to_string(int_value)); break;
    case Argument::Type::VARIABLE: output.append(var_value); break;
    case Argument::Type::REGISTER: output.append(register_to_string(reg_value)); break;
    }

    if (add_amount != 0) output.append("+" + std::to_string(add_amount));

    if (read_pointer) output.append("]");

    return output;
}

std::string Instruction::to_string() {
    std::string output;

    switch (type) {
    case Instruction::Type::PUSH: output.append("push"); break;
    case Instruction::Type::POP: output.append("pop"); break;
    case Instruction::Type::MOV: output.append("mov"); break;
    case Instruction::Type::MOVZX: output.append("movzx"); break;
    case Instruction::Type::SETE: output.append("sete"); break;
    case Instruction::Type::ADD: output.append("add"); break;
    case Instruction::Type::SUB: output.append("sub"); break;
    case Instruction::Type::IMUL: output.append("imul"); break;
    case Instruction::Type::DIV: output.append("div"); break;
    case Instruction::Type::INC: output.append("inc"); break;
    case Instruction::Type::DEC: output.append("dec"); break;
    case Instruction::Type::CMP: output.append("cmp"); break;
    case Instruction::Type::CMOVE: output.append("cmove"); break;
    case Instruction::Type::CMOVG: output.append("cmovg"); break;
    case Instruction::Type::CMOVL: output.append("cmovl"); break;
    case Instruction::Type::CMOVGE: output.append("cmovge"); break;
    case Instruction::Type::CMOVLE: output.append("cmovle"); break;
    case Instruction::Type::CMOVNE: output.append("cmovne"); break;
    case Instruction::Type::AND: output.append("and"); break;
    case Instruction::Type::OR: output.append("or"); break;
    case Instruction::Type::LABEL: return arg_name + ":"; break;
    case Instruction::Type::JZ: return "jz " + arg_name; break;
    case Instruction::Type::JNZ: return "jnz " + arg_name; break;
    case Instruction::Type::JMP: return "jmp " + arg_name; break;
    case Instruction::Type::TEST: output.append("test"); break;
    case Instruction::Type::XOR: output.append("xor"); break;
    case Instruction::Type::CALL: return "call " + arg_name; break;
    case Instruction::Type::SYSCALL: return "syscall"; break;
    case Instruction::Type::RET: return "ret"; break;
    }

    if (arguments.size() == 0) return output;

    output.append(" ");
    for (uint64_t i = 0; i < arguments.size() - 1; i++)
        output.append(arguments.at(i).to_string() + ", ");
    output.append(arguments.back().to_string());

    return output;
}
