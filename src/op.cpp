#include "include/op.h"

bool Op::is_prim_type_mode() {
    if (mode == Op::Mode::BIT_8 || mode == Op::Mode::BIT_64) return true;
    return false;
}

bool is_builtin_word(std::string word) { return Op::symbols_map.count(word); };
