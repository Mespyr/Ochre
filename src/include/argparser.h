#pragma once

#include <iostream>
#include <string>
#include "error.h"

class ArgParser {
public:
	ArgParser(int argc, const char* argv[]);
	void print_help();

	std::string input_filename();
	std::string output_filename();
	std::string asm_filename();
	std::string error_message();
	bool has_error();
	bool needs_help();
	
private:
	std::string program_name;
	std::string input_file;
    std::string output_file = "./a.out";
    std::string asm_file = "/tmp/out.asm";

	std::string error_msg;
	bool error = false;
    bool help = false;
};
