#pragma once

#include <fstream>
#include <iostream>
#include <string>

enum class FileMode { READ, WRITE };

class File {
  public:
    File(std::string filename, FileMode mode);
    ~File();
    bool operator==(bool t);

    bool        exists();
    void        writeln(std::string buf);
    std::string readln();

  private:
    std::fstream file;
};
