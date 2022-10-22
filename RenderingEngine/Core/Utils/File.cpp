#include "File.h"
#include <fstream>
#include <sstream>

std::string File::ReadFileToString(const std::string& file_name)
{
    std::ifstream file(file_name);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}
