#include "parser.h"

Op Parser::convert_token_to_op(
    Token tok, std::map<std::string, std::pair<LangType, int>> var_offsets) {
    static_assert(OP_COUNT == 58,
                  "unhandled op types in Parser::convert_token_to_op()");

    if (tok.type == TOKEN_WORD) {
        if (tok.value == "dump") return Op(tok.loc, OP_DUMP);
        // arithmetic
        else if (tok.value == "+")
            return Op(tok.loc, OP_PLUS);
        else if (tok.value == "-")
            return Op(tok.loc, OP_MINUS);
        else if (tok.value == "*")
            return Op(tok.loc, OP_MUL);
        else if (tok.value == "/")
            return Op(tok.loc, OP_DIV);
        // comparisons
        else if (tok.value == "=")
            return Op(tok.loc, OP_EQUAL);
        else if (tok.value == ">")
            return Op(tok.loc, OP_GREATER);
        else if (tok.value == "<")
            return Op(tok.loc, OP_LESS);
        else if (tok.value == ">=")
            return Op(tok.loc, OP_GREATER_EQ);
        else if (tok.value == "<=")
            return Op(tok.loc, OP_LESS_EQ);
        else if (tok.value == "!=")
            return Op(tok.loc, OP_NOT_EQ);
        else if (tok.value == "and")
            return Op(tok.loc, OP_AND);
        else if (tok.value == "or")
            return Op(tok.loc, OP_OR);
        // stack manipulation
        else if (tok.value == "pop")
            return Op(tok.loc, OP_POP);
        else if (tok.value == "dup")
            return Op(tok.loc, OP_DUP);
        else if (tok.value == "swp")
            return Op(tok.loc, OP_SWP);
        else if (tok.value == "rot")
            return Op(tok.loc, OP_ROT);
        else if (tok.value == "over")
            return Op(tok.loc, OP_OVER);
        // keywords
        else if (tok.value == "fun")
            return Op(tok.loc, OP_FUN);
        else if (tok.value == "const")
            return Op(tok.loc, OP_CONST);
        else if (tok.value == "end")
            return Op(tok.loc, OP_END);
        else if (tok.value == "struct")
            return Op(tok.loc, OP_STRUCT);
        else if (tok.value == "import")
            return Op(tok.loc, OP_IMPORT);
        else if (tok.value == "jmp")
            return Op(tok.loc, OP_JMP);
        else if (tok.value == "cjmpf")
            return Op(tok.loc, OP_CJMPF);
        else if (tok.value == "cjmpt")
            return Op(tok.loc, OP_CJMPT);
        else if (tok.value == "jmpe")
            return Op(tok.loc, OP_JMPE);
        else if (tok.value == "cjmpef")
            return Op(tok.loc, OP_CJMPEF);
        else if (tok.value == "cjmpet")
            return Op(tok.loc, OP_CJMPET);
        // syscalls
        else if (tok.value == "call0")
            return Op(tok.loc, OP_SYSCALL0);
        else if (tok.value == "call1")
            return Op(tok.loc, OP_SYSCALL1);
        else if (tok.value == "call2")
            return Op(tok.loc, OP_SYSCALL2);
        else if (tok.value == "call3")
            return Op(tok.loc, OP_SYSCALL3);
        else if (tok.value == "call4")
            return Op(tok.loc, OP_SYSCALL4);
        else if (tok.value == "call5")
            return Op(tok.loc, OP_SYSCALL5);
        else if (tok.value == "call6")
            return Op(tok.loc, OP_SYSCALL6);
        // other
        // OP_PUSH_TYPE_INST
        else if (tok.value.front() == '<' && tok.value.back() == '>') {
            std::string push_struct_name =
                parse_type_str(tok.value.substr(1, tok.value.size() - 2)).first;
            if (program.structs.count(push_struct_name) ||
                is_prim_type(push_struct_name)) {
                return Op(tok.loc, OP_PUSH_TYPE_INSTANCE, 1, push_struct_name);
            }
            uint64_t colon_pos = push_struct_name.find(':');
            // no chance to be an array
            if (colon_pos == std::string::npos) {
                print_error_at_loc(
                    tok.loc, "unknown type in 'push type instance' (<...>), '" +
                                 push_struct_name + "'");
                exit(1);
            }

            // we know there is a colon somewhere, might be an array
            std::string new_push_struct_name =
                push_struct_name.substr(0, colon_pos);
            std::string amount = push_struct_name.substr(colon_pos + 1);
            if (!program.structs.count(new_push_struct_name) &&
                !is_prim_type(new_push_struct_name)) {
                print_error_at_loc(
                    tok.loc, "unknown type in 'push type instance' (<...>), '" +
                                 new_push_struct_name + "'");
                exit(1);
            }
            // if the value is a constant
            if (program.consts.count(amount))
                return Op(tok.loc, OP_PUSH_TYPE_INSTANCE,
                          program.consts.at(amount).value,
                          new_push_struct_name);
            // if the value is a number
            if (lexer->is_number(amount)) {
                return Op(tok.loc, OP_PUSH_TYPE_INSTANCE, atol(amount.c_str()),
                          new_push_struct_name);
            }
            print_error_at_loc(tok.loc,
                               "expected size (integer) in 'push type "
                               "instance' (<...>), got '" +
                                   amount + "'");
            exit(1);

        }
        // OP_DELETE_PTR
        else if (tok.value == "delete")
            return Op(tok.loc, OP_DELETE_PTR);
        // OP_TYPE_CAST
        else if (tok.value.compare(0, 5, "cast:") == 0) {
            std::string                 full_type_str = tok.value.substr(5);
            std::pair<std::string, int> type = parse_type_str(full_type_str);
            // if the type is a struct and not a pointer
            if (program.structs.count(type.first) && type.second == 0) {
                print_error_at_loc(tok.loc,
                                   "cannot cast type to a non-pointer struct "
                                   "since those cannot exist on the stack");
                exit(1);

            } else if (is_prim_type(type.first) ||
                       program.structs.count(type.first))
                // if the struct is a primitive type or a struct pointer
                return Op(tok.loc, OP_TYPE_CAST, full_type_str);
            else {
                print_error_at_loc(tok.loc, "invalid type cast");
                exit(1);
            }
        }
        // OP_FUNCTION_CALL
        else if (program.functions.count(tok.value))
            return Op(tok.loc, OP_FUNCTION_CALL, tok.value);
        else if (program.consts.count(tok.value))
            return Op(tok.loc, OP_PUSH_INT, program.consts.at(tok.value).value);
        // OP_LABEL
        else if (tok.value.back() == ':') {
            if (tok.value.size() == 1) {
                print_error_at_loc(tok.loc,
                                   "unexpected ':' char found while parsing");
                exit(1);
            }
            tok.value.pop_back();
            return Op(tok.loc, OP_LABEL, tok.value);
        }
        // OP_SET
        else if (tok.value.front() == '@') {
            if (tok.value.size() == 1) {
                print_error_at_loc(tok.loc,
                                   "unexpected '@' char found while parsing");
                exit(1);
            }
            return Op(tok.loc, OP_SET, tok.value.substr(1));
        }
        // OP_READ
        else if (tok.value.front() == '&') {
            if (tok.value.size() == 1) {
                print_error_at_loc(tok.loc,
                                   "unexpected '&' char found while parsing");
                exit(1);
            }
            return Op(tok.loc, OP_READ, tok.value.substr(1));
        }

        // OP_DEFINE_VAR
        else if (is_prim_type(tok.value) || program.structs.count(tok.value) ||
                 is_pointer(tok.value))
            return Op(tok.loc, OP_DEFINE_VAR, tok.value);
        // OP_PUSH_VAR
        else if (var_offsets.count(tok.value))
            return Op(tok.loc, OP_PUSH_VAR, tok.value);

        else if (tok.value == "[" || tok.value == "]" || tok.value == "(" ||
                 tok.value == ")") {
            print_error_at_loc(tok.loc, "unexpected '" + tok.value +
                                            "' char found while parsing");
            exit(1);
        }
    } else if (tok.type == TOKEN_INT)
        return Op(tok.loc, OP_PUSH_INT, atol(tok.value.c_str()));
    else if (tok.type == TOKEN_STRING)
        return Op(
            tok.loc, OP_PUSH_STR,
            add_escapes_to_string(tok.value.substr(1, tok.value.length() - 2)));

    print_error_at_loc(tok.loc, "unknown word '" + tok.value + "'");
    exit(1);
}
