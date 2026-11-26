#pragma once

#include <fstream>
#include <iostream>
#include <string>

class File {
  public:
    enum class Mode { READ, WRITE };

    File(std::string filename, Mode mode);
    ~File();
    bool operator==(bool t);

    bool        exists();
    void        writeln(std::string buf);
    std::string readln();

  private:
    std::fstream file;
};
