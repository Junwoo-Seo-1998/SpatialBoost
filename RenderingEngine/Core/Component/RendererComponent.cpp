/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: RendererComponent.cpp
Purpose: RendererComponent for the engine to distinguish object when we render.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include "RendererComponent.h"

#include "Core/Data/Mesh.h"


FaceNormalLineRendererComponent::FaceNormalLineRendererComponent(std::shared_ptr<LineMesh> mesh)
	:mesh(mesh)
{
}

VertexNormalLineRendererComponent::VertexNormalLineRendererComponent(std::shared_ptr<LineMesh> mesh)
	:mesh(mesh)
{
}

FaceNormalMeshRendererComponent::FaceNormalMeshRendererComponent(std::shared_ptr<Mesh> mesh)
	:mesh(mesh)
{
}

VertexNormalMeshRendererComponent::VertexNormalMeshRendererComponent(std::shared_ptr<Mesh> mesh)
	:mesh(mesh)
{
}

LineRendererComponent::LineRendererComponent(std::shared_ptr<LineMesh> mesh)
	:mesh(mesh)
{
}
