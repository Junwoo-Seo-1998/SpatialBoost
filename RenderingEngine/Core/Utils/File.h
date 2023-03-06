#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs350_1
Author: Junwoo Seo, junwoo.seo, 0055213
End Header --------------------------------------------------------*/
#include <memory>
#include <string>
#include <vector>

struct TextureData;

class File
{
public:
	static std::string ReadFileToString(const std::string& file_name);
	static std::vector<std::string> ReadFileToStrings(const std::string& file_name);
	static std::shared_ptr<TextureData> ReadImageToTexture(const std::string& file_name);
};
