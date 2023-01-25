/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: VertexArray.cpp
Purpose: Abstracted class for vertex array object
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 10 2022
End Header --------------------------------------------------------*/
#include"VertexArray.h"
#include <glad.h>
VertexArray::~VertexArray()
{
	UnBind();
	glDeleteVertexArrays(1, &m_VertexArray);
}

std::shared_ptr<VertexArray> VertexArray::CreateVertexArray()
{
	return std::shared_ptr<VertexArray>(new VertexArray{});
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_VertexArray);
}

void VertexArray::UnBind() const
{
	glBindVertexArray(0);
}

VertexArray::VertexArray()
	:m_VertexArray(0)
{
	glCreateVertexArrays(1, &m_VertexArray);
}
