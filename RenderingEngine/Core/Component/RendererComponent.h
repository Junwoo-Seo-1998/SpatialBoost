#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: RendererComponent.h
Purpose: RendererComponent for the engine to distinguish object when we render.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include <memory>
class LineMesh;
class Mesh;

struct LineRendererComponent
{
	LineRendererComponent(std::shared_ptr<LineMesh> mesh);

	std::shared_ptr<LineMesh> mesh;
};

struct FaceNormalLineRendererComponent
{
	FaceNormalLineRendererComponent(std::shared_ptr<LineMesh> mesh);

	std::shared_ptr<LineMesh> mesh;
};
struct VertexNormalLineRendererComponent
{
	VertexNormalLineRendererComponent(std::shared_ptr<LineMesh> mesh);

	std::shared_ptr<LineMesh> mesh;
};
struct FaceNormalMeshRendererComponent
{
	FaceNormalMeshRendererComponent(std::shared_ptr<Mesh> mesh);

	std::shared_ptr<Mesh> mesh;
};
struct VertexNormalMeshRendererComponent
{
	VertexNormalMeshRendererComponent(std::shared_ptr<Mesh> mesh);

	std::shared_ptr<Mesh> mesh;
};


struct RendererComponent
{
	bool enabled = true;
};



