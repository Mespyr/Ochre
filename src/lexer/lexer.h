#pragma once

#include <assert.h>

#include <cstdint>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include "../include/error.h"
#include "../include/file.h"
#include "../include/token.h"

class Lexer {
  public:
    std::vector<Token> tokens;

    void set_file(std::string filename);
    void tokenize();

  private:
    std::string filename;
    std::string line;
    uint64_t    line_number = 0;

    uint64_t find_next_token_start_col(uint64_t column_number);
    uint64_t find_token_end_col(uint64_t column_number);
    uint64_t find_string_end_col(uint64_t column_number);
    void     tokenize_line();
};

// util
bool is_number(std::string n);
