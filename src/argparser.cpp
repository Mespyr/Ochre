#include "include/argparser.h"

ArgParser::ArgParser(int argc, const char* argv[]) {
    program_name = argv[0];
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help")
            help = true;
        else if (arg == "-o") {
            if (i + 1 < argc)
                output_file = argv[++i];  // Increment i to skip the value
            else {
                error_msg = "-o requires a filename argument.";
                error = true;
                return;
            }
        } else if (arg == "--asm") {
            if (i + 1 < argc) {
                asm_file = argv[++i];
            } else {
                error_msg = "--asm requires a filename argument.";
                error = true;
                return;
            }
        } else {
            // If it doesn't match a flag, treat it as the input file
            if (input_file.empty()) {
                input_file = arg;
            } else {
                error_msg = "unknown argument: " + arg + ".";
                error = true;
                return;
            }
        }
    }

    // no input file found :(
    if (input_file.empty() && !help) {
        error_msg = "no input file specified.";
        error = true;
    }
}

void ArgParser::print_help() {
    std::cout << "Ochre Version 2.0.0\n"
              << "Usage: " << program_name << " [options] <input_file>\n\n"
              << "Options:\n"
              << "\t-h, --help\tDisplay this information\n"
              << "\t-o <file>\tPlace the output into <file> (default: a.out)\n"
              << "\t--asm <file>\tOutput assembly code to <file> (default: "
                 "/tmp/out.asm)"
              << std::endl;
}

// getters
std::string ArgParser::input_filename() { return input_file; }
std::string ArgParser::output_filename() { return output_file; }
std::string ArgParser::asm_filename() { return asm_file; }
std::string ArgParser::error_message() { return error_msg; }
bool        ArgParser::has_error() { return error; }
bool        ArgParser::needs_help() { return help; }
