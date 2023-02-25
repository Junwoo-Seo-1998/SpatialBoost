#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Parser.h
Purpose: for reading obj files.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include <string>
#include"Core/Data/Mesh.h"

class BasicParser
{
public:
	virtual void ParseFile(const std::string& file_name, std::vector<glm::vec3>& loaded_points, std::vector<unsigned int>& loaded_indices) = 0;
};

class Parser
{
public:
	static void ParseFile(const std::string& file_name, std::vector<glm::vec3>& loaded_points, std::vector<unsigned int>& loaded_indices);

private:
	static std::unique_ptr<BasicParser> parser;
};

