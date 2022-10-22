#pragma once
#include <string>
class File
{
public:
	static std::string ReadFileToString(const std::string& file_name);
};
