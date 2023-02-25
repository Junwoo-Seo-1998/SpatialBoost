/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Parser.cpp
Purpose: for reading obj files.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include "Parser.h"
#include <string>
#include <fstream>
#include <sstream>
#include"Core/Utils/Math.h"
#include "Core/Utils/ManualParser.h"
#include "Core/Utils/AssimpParser.h"
//std::unique_ptr<BasicParser> Parser::parser{ new ManualParser() };
std::unique_ptr<BasicParser> Parser::parser{ new AssimpParser() };

void Parser::ParseFile(const std::string& file_name, std::vector<glm::vec3>& loaded_points,
	std::vector<unsigned>& loaded_indices)
{
	parser->ParseFile(file_name, loaded_points, loaded_indices);
}

