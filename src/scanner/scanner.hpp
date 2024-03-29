#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "../error/error.hpp"
#include "../token.hpp"

class Scanner {
  public:
    std::unique_ptr<Error> error;
    Scanner(const std::string& filename);
    void insert(const std::string& filename);

    Token peek();
    Token next();
    bool  eof();

  private:
    std::vector<Token> token_stream;
    std::string        current_file;
    size_t             stream_index = 0;

    uint32_t find_start_col(const std::string& line, uint32_t column_number);
    uint32_t find_end_col(const std::string& line, uint32_t column_number);
    uint32_t find_string_end(const std::string& line, uint32_t column_number);
    uint32_t find_char_end(const std::string& line, uint32_t column_number);
    void     tokenize_line(const std::string& line, uint32_t line_num);

    Token::Type get_token_type(const std::string& token);

    bool is_number(std::string str);
};
