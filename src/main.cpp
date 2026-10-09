#include <stdio.h>
#include <unistd.h>

#include "compiler/compiler.h"
#include "include/argparser.h"
#include "include/error.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "type_checker/type_checker.h"

void exec(const char* cmd) {
    char  buffer[128];
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        print_error("popen() failed!");
        exit(1);
    }
    while (fgets(buffer, sizeof buffer, pipe) != nullptr);
    pclose(pipe);
}

int main(int argc, const char* argv[]) {
    ArgParser args(argc, argv);

    if (args.has_error()) {
        print_error(args.error_message());
        std::cout << "\n";
        args.print_help();
        return 1;
    }

    if (args.needs_help()) {
        args.print_help();
        return 0;
    }

    Lexer lexer;
    lexer.set_file(args.input_filename());
    lexer.tokenize();

    Parser parser(&lexer);
    parser.parse();

    TypeChecker type_checker(parser.program);
    type_checker.verify();
    type_checker.perform_checks();

    Compiler compiler(type_checker.program);
    compiler.generate_asm();
    compiler.perform_optimizations();
    compiler.write_asm_to_file(args.asm_filename());

    const std::string& compile_command =
        "fasm " + args.asm_filename() + " " + args.output_filename();
    exec(compile_command.c_str());
    return 0;
}
