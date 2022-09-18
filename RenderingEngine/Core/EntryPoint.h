/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: EntryPoint.h
Purpose: entry point for the engine to make follow init order.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#pragma once
#include"Core/Application.h"
#include<memory>
#include<iostream>
extern std::shared_ptr<Application> CoreMain();
int main(int /*argc*/, char** /*argv*/) 
{
	std::shared_ptr<Application> application = CoreMain();
	if (!application->Init())
	{
		std::cout << "error on init!" << std::endl;
		return -1;
	}
	application->UserDefinedInit();
	application->Update();
	application->Close();
	return 0;
}
