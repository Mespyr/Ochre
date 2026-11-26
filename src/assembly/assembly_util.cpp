#include "assembly.h"

std::string register_to_string(Register reg) {
    std::string output;

    switch (reg) {
    case Register::RAX: return "rax";
    case Register::RBX: return "rbx";
    case Register::RCX: return "rcx";
    case Register::RSP: return "rsp";
    case Register::RDI: return "rdi";
    case Register::RSI: return "rsi";
    case Register::RDX: return "rdx";
    case Register::R10: return "r10";
    case Register::R8: return "r8";
    case Register::R9: return "r9";

    case Register::AL: return "al";
    case Register::BL: return "bl";
    case Register::CL: return "cl";
    case Register::DL: return "dl";
    }
}

void AssemblyProgram::start_new_function(std::string func_name) {
    code.insert({func_name, {}});
    current_func_name = func_name;
}

void AssemblyProgram::new_inst(Instruction inst) { code.at(current_func_name).push_back(inst); }
