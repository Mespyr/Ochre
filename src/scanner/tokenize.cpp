#include "scanner.hpp"

void Scanner::tokenize_line(Location loc) {
    const std::string &line = loc.file()->at(loc.line_num());

    while (loc.start_col() < line.length()) {
        char        c = line.at(loc.start_col());
        std::string token_str;
        Token::Type token_type;

        if (c == '#')
            return;
        else if (c == '\'') {
            loc.set_end_col(find_char_end(line, loc.start_col()));
            if (loc.end_col() >= line.length()) {
                error = std::make_unique<LocationError>(
                    loc, "unexpected EOL while tokenizing char-string");
                return;
            } else if (line.at(loc.end_col()) != '\'') {
                error = std::make_unique<LocationError>(
                    loc, "unexpected char found while tokenizing char-string");
                return;
            }
            token_type = Token::CHAR;
            loc.set_end_col(loc.end_col() + 1);
        } else if (c == '"') {
            loc.set_end_col(find_string_end(line, loc.start_col()));
            if (loc.end_col() >= line.length()) {
                error = std::make_unique<LocationError>(
                    loc, "unexpected EOL while tokenizing string");
                return;
            }
            token_type = Token::STRING;
            loc.set_end_col(loc.end_col() + 1);
        } else if (Token::single_chars.count(c)) {
            token_type = Token::single_chars.at(c);
            loc.set_end_col(loc.start_col() + 1);
        } else {
            loc.set_end_col(find_end_col(line, loc.start_col()));
            std::string token_str =
                line.substr(loc.start_col(), loc.end_col() - loc.start_col());
            token_type = get_token_type(token_str);
            // means were either dealing with a float or an error
            if (token_type == Token::NUMBER && loc.end_col() < line.length() &&
                line.at(loc.end_col()) == '.') {
                loc.set_end_col(loc.end_col() + 1);
                if (loc.end_col() >= line.length() ||
                    std::isspace(line.at(loc.end_col()))) {
                    error = std::make_unique<LocationError>(
                        loc, "unexpected '.' found while tokenizing number");
                    return;
                }
                uint32_t old_end_column = loc.end_col();
                loc.set_end_col(find_end_col(line, loc.end_col()));
                std::string float_decimal_section =
                    line.substr(old_end_column, loc.end_col() - old_end_column);
                if (get_token_type(float_decimal_section) != Token::NUMBER ||
                    float_decimal_section.front() == '-') {
                    loc.set_start_col(old_end_column - 1);
                    error = std::make_unique<LocationError>(
                        loc, "unexpected '.' found while tokenizing number");
                    return;
                }
                token_type = Token::FLOATING_POINT;
            }
        }

        token_stream.insert(
            token_stream.begin() + stream_index,
            Token(line.substr(loc.start_col(), loc.end_col() - loc.start_col()),
                  token_type, loc));

        stream_index++;
        loc.set_start_col(find_start_col(line, loc.end_col()));
    }
}
