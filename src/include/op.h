#pragma once

#include <assert.h>

#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "location.h"
#include "token.h"

class Op {
  public:
    enum class Mode {
        BIT_8,
        BIT_64,
        STRUCT,
    };

    enum class Type {
        // debugging
        DUMP,
        // arithmetics
        PLUS,
        MINUS,
        MUL,
        DIV,
        // comparisons
        EQUAL,
        GREATER,
        LESS,
        GREATER_EQ,
        LESS_EQ,
        NOT_EQ,
        AND,
        OR,
        NOT,
        // stack manipulation
        POP,
        DUP,   // [a] -> [a, a]
        SWP,   // [a, b] -> [b, a]
        ROT,   // [a, b, c] -> [b, c, a]
        OVER,  // [a, b] -> [a, b a]
        // keywords
        FUN,
        END,
        STRUCT,
        CONST,
        IMPORT,
        // variables
        DEFINE_VAR,
        SET,
        READ,

        SET_VAR,
        SET_VAR_FROM_OTHER_PTR,
        SET_VAR_STRUCT_MEMBER,
        READ_VAR,
        READ_VAR_STRUCT_MEMBER,
        PUSH_VAR,

        SET_PTR,
        SET_PTR_FROM_OTHER_PTR,
        SET_PTR_STRUCT_MEMBER,
        READ_PTR,
        READ_PTR_STRUCT_MEMBER,

        // syscalls
        SYSCALL0,
        SYSCALL1,
        SYSCALL2,
        SYSCALL3,
        SYSCALL4,
        SYSCALL5,
        SYSCALL6,
        // labels
        LABEL,
        LABEL_END,
        JMP,
        CJMPT,
        CJMPF,
        JMPE,
        CJMPET,
        CJMPEF,
        // other
        PUSH_INT,
        PUSH_CHAR,
        PUSH_STR,
        PUSH_ARGV,
        PUSH_TYPE_INSTANCE,
        DELETE_PTR,
        TYPE_CAST,
        FUNCTION_CALL,
        RETURN,
        // count | must always be last
        COUNT
    };

    static inline const std::unordered_map<std::string, Type> symbols_map = {
        {"dump", Type::DUMP},      {"+", Type::PLUS},
        {"-", Type::MINUS},        {"*", Type::MUL},
        {"/", Type::DIV},          {"=", Type::EQUAL},
        {">", Type::GREATER},      {"<", Type::LESS},
        {">=", Type::GREATER_EQ},  {"<=", Type::LESS_EQ},
        {"!=", Type::NOT_EQ},      {"and", Type::AND},
        {"or", Type::OR},          {"not", Type::NOT},
        {"pop", Type::POP},        {"dup", Type::DUP},
        {"swp", Type::SWP},        {"rot", Type::ROT},
        {"over", Type::OVER},      {"call0", Type::SYSCALL0},
        {"call1", Type::SYSCALL1}, {"call2", Type::SYSCALL2},
        {"call3", Type::SYSCALL3}, {"call4", Type::SYSCALL4},
        {"call5", Type::SYSCALL5}, {"call6", Type::SYSCALL6},
        {"fun", Type::FUN},        {"const", Type::CONST},
        {"end", Type::END},        {"struct", Type::STRUCT},
        {"import", Type::IMPORT},  {"jmp", Type::JMP},
        {"cjmpt", Type::CJMPT},    {"cjmpf", Type::CJMPF},
        {"jmpe", Type::JMPE},      {"cjmpet", Type::CJMPET},
        {"cjmpef", Type::CJMPEF},  {"delete", Type::DELETE_PTR},
        {"argv", Type::PUSH_ARGV}, {"return", Type::RETURN}};

    Op(Location loc, Type type) : loc(loc), type(type) {}

    Op(Location loc, Type type, int64_t int_operand)
        : loc(loc), type(type), int_operand(int_operand) {}

    Op(Location loc, Type type, std::string str_operand)
        : loc(loc), type(type), str_operand(str_operand) {}

    Op(Location loc, Type type, int64_t int_operand, std::string str_operand)
        : loc(loc), type(type), int_operand(int_operand), str_operand(str_operand) {}

    bool is_prim_type_mode();

    Location loc;
    Type     type;

    int64_t     int_operand;
    int64_t     int_operand_2;
    std::string str_operand;
    Mode        mode;
};

bool is_builtin_word(std::string word);
