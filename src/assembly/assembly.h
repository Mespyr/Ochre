#pragma once

#include <cstdint>
#include <cstdlib>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "../include/file.h"

// all used registers (as far as I know)
enum class Register {
    RAX,
    RBX,
    RCX,
    RSP,
    RDI,
    RSI,
    RDX,
    R10,
    R8,
    R9,

    AL,
    BL,
    CL,
    DL,
};
std::string register_to_string(Register reg);

enum AsmSizeInference { INFER_QWORD, INFER_BYTE, INFER_NONE };

class Argument {
  public:
    enum class Type { VARIABLE, INTEGER, REGISTER };

    Argument(std::string value, bool read_pointer = false,
             AsmSizeInference size_inference = INFER_NONE, int64_t add_amount = 0)
        : read_pointer(read_pointer),
          size_inference(size_inference),
          type(Type::VARIABLE),
          var_value(value),
          add_amount(add_amount) {}

    Argument(int64_t value, bool read_pointer = false, AsmSizeInference size_inference = INFER_NONE,
             int64_t add_amount = 0)
        : read_pointer(read_pointer),
          size_inference(size_inference),
          type(Type::INTEGER),
          int_value(value),
          add_amount(add_amount) {}

    Argument(Register value, bool read_pointer = false,
             AsmSizeInference size_inference = INFER_NONE, int64_t add_amount = 0)
        : read_pointer(read_pointer),
          size_inference(size_inference),
          type(Type::REGISTER),
          reg_value(value),
          add_amount(add_amount) {}

    bool             read_pointer = false;
    AsmSizeInference size_inference;

    Type        type;
    std::string var_value;
    int64_t     int_value;
    Register reg_value;

    int64_t add_amount;

    std::string to_string();
};

class Instruction {
  public:
    enum class Type {
        PUSH,
        POP,
        MOV,
        MOVZX,
        SETE,

        ADD,
        SUB,
        IMUL,
        DIV,
        INC,
        DEC,

        CMP,
        CMOVE,
        CMOVG,
        CMOVL,
        CMOVGE,
        CMOVLE,
        CMOVNE,
        AND,
        OR,
        TEST,
        XOR,

        LABEL,  // not an instruction, but an indicator to place a label
        JZ,
        JNZ,
        JMP,

        CALL,
        SYSCALL,
        RET
    };

    Instruction(Type t) : type(t) {}

    Instruction(Type t, std::string arg_name) : type(t), arg_name(arg_name) {}

    Type                  type;
    std::vector<Argument> arguments;
    std::string           arg_name;  // used by LABEL, JZ, JNZ, JMP, CALL

    std::string to_string();
};

class AssemblyProgram {
  public:
    bool                                            compile_dump = false;
    std::map<std::string, std::vector<Instruction>> code;
    std::vector<std::string>                        string_data;

    void start_new_function(std::string func_name);
    void new_inst(Instruction inst);

    void write_to_file(std::string filename);

    Instruction inst_push(Argument arg);
    Instruction inst_pop(Argument arg);
    Instruction inst_mov(Argument arg1, Argument arg2);
    Instruction inst_movzx(Argument arg1, Argument arg2);
    Instruction inst_sete(Argument arg);

    Instruction inst_add(Argument arg1, Argument arg2);
    Instruction inst_sub(Argument arg1, Argument arg2);
    Instruction inst_imul(Argument arg1, Argument arg2);
    Instruction inst_div(Argument arg);
    Instruction inst_inc(Argument arg);
    Instruction inst_dec(Argument arg);

    Instruction inst_cmp(Argument arg1, Argument arg2);
    Instruction inst_cmove(Argument arg1, Argument arg2);
    Instruction inst_cmovg(Argument arg1, Argument arg2);
    Instruction inst_cmovl(Argument arg1, Argument arg2);
    Instruction inst_cmovge(Argument arg1, Argument arg2);
    Instruction inst_cmovle(Argument arg1, Argument arg2);
    Instruction inst_cmovne(Argument arg1, Argument arg2);
    Instruction inst_and(Argument arg1, Argument arg2);
    Instruction inst_or(Argument arg1, Argument arg2);
    Instruction inst_xor(Argument arg1, Argument arg2);
    Instruction inst_test(Argument arg1, Argument arg2);

    Instruction inst_label(std::string label_name);
    Instruction inst_jz(std::string label_name);
    Instruction inst_jnz(std::string label_name);
    Instruction inst_jmp(std::string label_name);

    Instruction inst_call(std::string func_name);
    Instruction inst_syscall();
    Instruction inst_ret();

  private:
    std::string current_func_name;
    void        write_beginning_boilerplate(File* outfile);

    Instruction inst_1_arity_backend(Instruction::Type type, Argument arg);
    Instruction inst_2_arity_backend(Instruction::Type type, Argument arg1, Argument arg2);
    Instruction inst_name_arg_backend(Instruction::Type type, std::string name);
};
