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



VertexArray::VertexArray()
	:m_VertexArray(0)
{
}

VertexArray::~VertexArray()
{
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &m_VertexArray);
}

void VertexArray::AttachBuffer(const VertexBuffer& buffer, bool ManualVAO)
{
	if(!ManualVAO)
		Bind();
	const DescribedData& data_descriptions = buffer.GetDescribedData();
	buffer.Bind();
	for (const auto& description : data_descriptions)
	{
		glEnableVertexAttribArray(description.m_LayoutLocation);
		const void * pointer = static_cast<char*>(nullptr) + description.m_Offset;
		glVertexAttribPointer(description.m_LayoutLocation, static_cast<GLint>(description.m_ElementCount), 
			description.ShaderDataTypeToOpenGLBaseType(),
			description.m_Normalize ? GL_TRUE : GL_FALSE,
			static_cast<GLsizei>(data_descriptions.GetStride()),
			pointer);
	}
}

void VertexArray::AttachBuffer(const ElementBuffer& buffer, bool ManualVAO)
{
	if (!ManualVAO)
		Bind();
	buffer.Bind();
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_VertexArray);
}

void VertexArray::UnBind() const
{
	glBindVertexArray(0);
}

void VertexArray::CreateVertexArray()
{
	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);
}
