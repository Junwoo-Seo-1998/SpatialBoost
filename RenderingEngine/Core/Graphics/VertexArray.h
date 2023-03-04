#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: VertexArray.h
Purpose: Abstracted class for vertex array object
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 10 2022
End Header --------------------------------------------------------*/
#include <memory>

class VertexArray
{
public:
	virtual ~VertexArray();
	static std::shared_ptr<VertexArray> CreateVertexArray();
	//void AttachBuffer(const VertexBuffer& buffer, bool ManualVAO = false);
	//void AttachBuffer(const IndexBuffer& buffer, bool ManualVAO = false);

	void Bind() const;
	void UnBind() const;
private:
	VertexArray();
private:
	unsigned m_VertexArray;
};