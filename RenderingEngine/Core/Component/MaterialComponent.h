#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_2
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include <string>
#include "Core/Data/Material.h"
#include "Core/UUID.h"
enum class RenderMode
{
	Deferred = 0,
	Forward
};
struct MaterialComponent
{
	MaterialComponent() = default;
	MaterialComponent(const std::string& shader);
	UUID shader = UUID::null;
	RenderMode mode = RenderMode::Deferred;
	Material material;
	MaterialData& operator[](const std::string& key);
};
