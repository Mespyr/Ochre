#include "parser.h"

Op Parser::convert_token_to_op(Token                                           tok,
                               std::map<std::string, std::pair<LangType, int>> var_offsets) {
    static_assert(std::to_underlying(Op::Type::COUNT) == 57,
                  "unhandled op types in Parser::convert_token_to_op()");

    if (tok.type == Token::Type::WORD) {
        if (Op::symbols_map.count(tok.value)) return Op(tok.loc, Op::symbols_map.at(tok.value));
        // other
        // Op::Type::PUSH_TYPE_INST
        else if (tok.value.front() == '<' && tok.value.back() == '>') {
            std::string push_struct_name =
                parse_type_str(tok.value.substr(1, tok.value.size() - 2)).first;
            if (program.structs.count(push_struct_name) || is_prim_type(push_struct_name)) {
                return Op(tok.loc, Op::Type::PUSH_TYPE_INSTANCE, 1, push_struct_name);
            }
            uint64_t colon_pos = push_struct_name.find(':');
            // no chance to be an array
            if (colon_pos == std::string::npos) {
                print_error_at_loc(tok.loc, "unknown type in 'push type instance' (<...>), '" +
                                                push_struct_name + "'");
                exit(1);
            }

            // we know there is a colon somewhere, might be an array
            std::string new_push_struct_name = push_struct_name.substr(0, colon_pos);
            std::string amount = push_struct_name.substr(colon_pos + 1);
            if (!program.structs.count(new_push_struct_name) &&
                !is_prim_type(new_push_struct_name)) {
                print_error_at_loc(tok.loc, "unknown type in 'push type instance' (<...>), '" +
                                                new_push_struct_name + "'");
                exit(1);
            }
            // if the value is a constant
            if (program.consts.count(amount))
                return Op(tok.loc, Op::Type::PUSH_TYPE_INSTANCE, program.consts.at(amount).value,
                          new_push_struct_name);
            // if the value is a number
            if (is_number(amount)) {
                return Op(tok.loc, Op::Type::PUSH_TYPE_INSTANCE, atol(amount.c_str()),
                          new_push_struct_name);
            }
            print_error_at_loc(tok.loc,
                               "expected size (integer) in 'push type "
                               "instance' (<...>), got '" +
                                   amount + "'");
            exit(1);

        }
        // Op::Type::TYPE_CAST
        else if (tok.value.compare(0, 5, "cast:") == 0) {
            std::string                 full_type_str = tok.value.substr(5);
            std::pair<std::string, int> type = parse_type_str(full_type_str);
            // if the type is a struct and not a pointer
            if (program.structs.count(type.first) && type.second == 0) {
                print_error_at_loc(tok.loc,
                                   "cannot cast type to a non-pointer struct "
                                   "since those cannot exist on the stack");
                exit(1);

            } else if (is_prim_type(type.first) || program.structs.count(type.first))
                // if the struct is a primitive type or a struct pointer
                return Op(tok.loc, Op::Type::TYPE_CAST, full_type_str);
            else {
                print_error_at_loc(tok.loc, "invalid type cast");
                exit(1);
            }
        }
        // Op::Type::FUNCTION_CALL
        else if (program.functions.count(tok.value))
            return Op(tok.loc, Op::Type::FUNCTION_CALL, tok.value);
        else if (program.consts.count(tok.value))
            return Op(tok.loc, Op::Type::PUSH_INT, program.consts.at(tok.value).value);
        // Op::Type::LABEL
        else if (tok.value.back() == ':') {
            if (tok.value.size() == 1) {
                print_error_at_loc(tok.loc, "unexpected ':' char found while parsing");
                exit(1);
            }
            tok.value.pop_back();
            return Op(tok.loc, Op::Type::LABEL, tok.value);
        }
        // Op::Type::SET
        else if (tok.value.front() == '@') {
            if (tok.value.size() == 1) {
                print_error_at_loc(tok.loc, "unexpected '@' char found while parsing");
                exit(1);
            }
            return Op(tok.loc, Op::Type::SET, tok.value.substr(1));
        }
        // Op::Type::READ
        else if (tok.value.front() == '&') {
            if (tok.value.size() == 1) {
                print_error_at_loc(tok.loc, "unexpected '&' char found while parsing");
                exit(1);
            }
            return Op(tok.loc, Op::Type::READ, tok.value.substr(1));
        }

        // Op::Type::DEFINE_VAR
        else if (is_prim_type(tok.value) || program.structs.count(tok.value) ||
                 is_pointer(tok.value))
            return Op(tok.loc, Op::Type::DEFINE_VAR, tok.value);
        // OP::Type::PUSH_VAR
        else if (var_offsets.count(tok.value))
            return Op(tok.loc, Op::Type::PUSH_VAR, tok.value);

        else if (tok.value == "[" || tok.value == "]" || tok.value == "(" || tok.value == ")") {
            print_error_at_loc(tok.loc, "unexpected '" + tok.value + "' char found while parsing");
            exit(1);
        }
    } else if (tok.type == Token::Type::INT)
        return Op(tok.loc, Op::Type::PUSH_INT, atol(tok.value.c_str()));
    else if (tok.type == Token::Type::CHAR) {
        // escape sequence char token (since a length of 3 is one char + 2 "'"s
        std::string val = tok.value.substr(1, tok.value.length() - 2);
        if (tok.value.length() > 3) {
            std::string raw = parse_single_escape_sequence(val);
            if (raw.empty()) {
                print_error_at_loc(tok.loc,
                                   "unexpected '" + tok.value + "' char found while parsing");
                exit(1);
            }
            val = raw;
        }
        // this should only be one character
        if (val.length() != 1) {
            print_error_at_loc(tok.loc,
                               "error in Parser::convert_token_to_op(): char "
                               "managed to be more than one byte long");
            exit(1);
        }
        return Op(tok.loc, Op::Type::PUSH_CHAR, val.at(0), val);
    } else if (tok.type == Token::Type::STRING)
        return Op(tok.loc, Op::Type::PUSH_STR,
                  add_escapes_to_string(tok.value.substr(1, tok.value.length() - 2)));

    print_error_at_loc(tok.loc, "unknown word '" + tok.value + "'");
    exit(1);
}
