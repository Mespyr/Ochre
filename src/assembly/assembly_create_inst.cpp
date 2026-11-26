#include "assembly.h"

// backend
Instruction AssemblyProgram::inst_1_arity_backend(Instruction::Type type, Argument arg) {
    Instruction i(type);
    i.arguments.push_back(arg);
    return i;
}

Instruction AssemblyProgram::inst_2_arity_backend(Instruction::Type type, Argument arg1,
                                                  Argument arg2) {
    Instruction i(type);
    i.arguments.push_back(arg1);
    i.arguments.push_back(arg2);
    return i;
}

Instruction AssemblyProgram::inst_name_arg_backend(Instruction::Type type, std::string name) {
    return Instruction(type, name);
}

// frontent
Instruction AssemblyProgram::inst_push(Argument arg) {
    return inst_1_arity_backend(Instruction::Type::PUSH, arg);
}

Instruction AssemblyProgram::inst_pop(Argument arg) {
    return inst_1_arity_backend(Instruction::Type::POP, arg);
}

Instruction AssemblyProgram::inst_mov(Argument arg1, Argument arg2) {
    return inst_2_arity_backend(Instruction::Type::MOV, arg1, arg2);
}

Instruction AssemblyProgram::inst_movzx(Argument arg1, Argument arg2) {
    return inst_2_arity_backend(Instruction::Type::MOVZX, arg1, arg2);
}

Instruction AssemblyProgram::inst_sete(Argument arg) {
    return inst_1_arity_backend(Instruction::Type::SETE, arg);
}

Instruction AssemblyProgram::inst_add(Argument arg1, Argument arg2) {
    return inst_2_arity_backend(Instruction::Type::ADD, arg1, arg2);
}

Instruction AssemblyProgram::inst_sub(Argument arg1, Argument arg2) {
    return inst_2_arity_backend(Instruction::Type::SUB, arg1, arg2);
}

Instruction AssemblyProgram::inst_imul(Argument arg1, Argument arg2) {
    return inst_2_arity_backend(Instruction::Type::IMUL, arg1, arg2);
}

Instruction AssemblyProgram::inst_div(Argument arg) {
    return inst_1_arity_backend(Instruction::Type::DIV, arg);
}

Instruction AssemblyProgram::inst_inc(Argument arg) {
    return inst_1_arity_backend(Instruction::Type::INC, arg);
}

Instruction AssemblyProgram::inst_dec(Argument arg) {
    return inst_1_arity_backend(Instruction::Type::DEC, arg);
}

Instruction AssemblyProgram::inst_cmp(Argument arg1, Argument arg2) {
    return inst_2_arity_backend(Instruction::Type::CMP, arg1, arg2);
}

Instruction AssemblyProgram::inst_cmove(Argument arg1, Argument arg2) {
    return inst_2_arity_backend(Instruction::Type::CMOVE, arg1, arg2);
}

Instruction AssemblyProgram::inst_cmovg(Argument arg1, Argument arg2) {
    return inst_2_arity_backend(Instruction::Type::CMOVG, arg1, arg2);
}

Instruction AssemblyProgram::inst_cmovl(Argument arg1, Argument arg2) {
    return inst_2_arity_backend(Instruction::Type::CMOVL, arg1, arg2);
}

Instruction AssemblyProgram::inst_cmovge(Argument arg1, Argument arg2) {
    return inst_2_arity_backend(Instruction::Type::CMOVGE, arg1, arg2);
}

Instruction AssemblyProgram::inst_cmovle(Argument arg1, Argument arg2) {
    return inst_2_arity_backend(Instruction::Type::CMOVLE, arg1, arg2);
}

Instruction AssemblyProgram::inst_cmovne(Argument arg1, Argument arg2) {
    return inst_2_arity_backend(Instruction::Type::CMOVNE, arg1, arg2);
}

Instruction AssemblyProgram::inst_and(Argument arg1, Argument arg2) {
    return inst_2_arity_backend(Instruction::Type::AND, arg1, arg2);
}

Instruction AssemblyProgram::inst_or(Argument arg1, Argument arg2) {
    return inst_2_arity_backend(Instruction::Type::OR, arg1, arg2);
}

Instruction AssemblyProgram::inst_xor(Argument arg1, Argument arg2) {
    return inst_2_arity_backend(Instruction::Type::XOR, arg1, arg2);
}

Instruction AssemblyProgram::inst_test(Argument arg1, Argument arg2) {
    return inst_2_arity_backend(Instruction::Type::TEST, arg1, arg2);
}

Instruction AssemblyProgram::inst_label(std::string label_name) {
    return inst_name_arg_backend(Instruction::Type::LABEL, label_name);
}

Instruction AssemblyProgram::inst_jz(std::string label_name) {
    return inst_name_arg_backend(Instruction::Type::JZ, label_name);
}

Instruction AssemblyProgram::inst_jnz(std::string label_name) {
    return inst_name_arg_backend(Instruction::Type::JNZ, label_name);
}

Instruction AssemblyProgram::inst_jmp(std::string label_name) {
    return inst_name_arg_backend(Instruction::Type::JMP, label_name);
}

Instruction AssemblyProgram::inst_call(std::string func_name) {
    return inst_name_arg_backend(Instruction::Type::CALL, func_name);
}

Instruction AssemblyProgram::inst_syscall() { return Instruction(Instruction::Type::SYSCALL); }

Instruction AssemblyProgram::inst_ret() { return Instruction(Instruction::Type::RET); }
