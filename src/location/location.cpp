#include "location.hpp"

std::shared_ptr<File> Location::file() { return _file; }
uint32_t              Location::line_num() { return line_number; }
uint32_t              Location::start_col() { return start_column; }
uint32_t              Location::end_col() { return end_column; }

void Location::set_line_num(uint32_t n) { line_number = n; }
void Location::set_start_col(uint32_t n) { start_column = n; }
void Location::set_end_col(uint32_t n) { end_column = n; }
