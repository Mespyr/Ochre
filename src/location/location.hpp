#pragma once

#include <cstdint>
#include <memory>

#include "../file/file.hpp"

class Location {
  public:
    Location(std::shared_ptr<File> file, uint32_t line_num,
             uint32_t start_column, uint32_t end_column)
        : _file(file),
          line_number(line_num),
          start_column(start_column),
          end_column(end_column) {}

    std::shared_ptr<File> file();
    uint32_t              line_num();
    uint32_t              start_col();
    uint32_t              end_col();

    void set_line_num(uint32_t n);
    void set_start_col(uint32_t n);
    void set_end_col(uint32_t n);

  private:
    std::shared_ptr<File> _file;
    uint32_t              line_number, start_column, end_column;
};
