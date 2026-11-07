#include "include/file.h"

File::File(std::string filename, FileMode mode) {
    switch (mode) {
    case FileMode::WRITE: file.open(filename, std::ios::out); break;
    case FileMode::READ: file.open(filename, std::ios::in); break;
    }
}

File::~File() { file.close(); }

bool File::operator==(bool t) { return (bool)file == t; }

bool File::exists() { return static_cast<bool>(file); }

void File::writeln(std::string buf) {
    buf.push_back('\n');
    file.write(buf.c_str(), buf.length());
}

std::string File::readln() {
    if (!file) return "";
    std::string line;
    std::getline(file, line);
    return line;
}
