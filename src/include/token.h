#pragma once

#include <string>

#include "location.h"

class Token {
  public:
    enum class Type { WORD, INT, CHAR, STRING, COUNT };

    Token(std::string value, Type type, Location loc) : value(value), type(type), loc(loc) {}

    std::string value;
    Type        type;
    Location    loc;
};
