/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Buffer.cpp
Purpose: Abstracted classes for vertex buffer object and index buffer with helper classes
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 10 2022
End Header --------------------------------------------------------*/
#include"Buffer.h"

DataAndLayoutLocation::DataAndLayoutLocation(unsigned layout_location, DataType data, bool normalize)
	:m_DataType(data), m_LayoutLocation(layout_location), m_Size(0), m_ElementCount(0), m_Offset(0), m_Normalize(normalize)
{
	//compute size of data 
	switch (data)
	{
	case DataType::Bool:	m_Size = 4;						 break;//for padding
	case DataType::Int:		m_Size = sizeof(int);			 break;
	case DataType::Int2:	m_Size = sizeof(int) * 2;		 break;
	case DataType::Int3:	m_Size = sizeof(int) * 3;		 break;
	case DataType::Int4:	m_Size = sizeof(int) * 4;		 break;
	case DataType::Float:   m_Size = sizeof(float);			 break;
	case DataType::Float2:	m_Size = sizeof(float) * 2;		 break;
	case DataType::Float3:	m_Size = sizeof(float) * 3;		 break;
	case DataType::Float4:	m_Size = sizeof(float) * 4;		 break;
	case DataType::Mat3:	m_Size = sizeof(float) * 3 * 3;	 break;
	case DataType::Mat4:	m_Size = sizeof(float) * 4 * 4;	 break;
	}

	//get element count 
	switch (data)
	{
	case DataType::Bool:	m_ElementCount = 1; 	break;
	case DataType::Int:		m_ElementCount = 1;		break;
	case DataType::Int2:	m_ElementCount = 2;		break;
	case DataType::Int3:	m_ElementCount = 3;		break;
	case DataType::Int4:	m_ElementCount = 4;		break;
	case DataType::Float:	m_ElementCount = 1;		break;
	case DataType::Float2:	m_ElementCount = 2;		break;
	case DataType::Float3:	m_ElementCount = 3;		break;
	case DataType::Float4:	m_ElementCount = 4;		break;
	case DataType::Mat3:	m_ElementCount = 3 * 3;	break;
	case DataType::Mat4:	m_ElementCount = 4 * 4;	break;
	}
}

GLenum DataAndLayoutLocation::ShaderDataTypeToOpenGLBaseType() const
{
	switch (m_DataType)
	{
	case DataType::Bool:		return GL_BOOL;
	case DataType::Int:			return GL_INT;
	case DataType::Int2:		return GL_INT;
	case DataType::Int3:		return GL_INT;
	case DataType::Int4:		return GL_INT;
	case DataType::Float:		return GL_FLOAT;
	case DataType::Float2:		return GL_FLOAT;
	case DataType::Float3:		return GL_FLOAT;
	case DataType::Float4:		return GL_FLOAT;
	case DataType::Mat3:		return GL_FLOAT;
	case DataType::Mat4:		return GL_FLOAT;
	}
	return 0;
}

DescribedData::DescribedData(const std::initializer_list<DataAndLayoutLocation>& described_data)
	:m_DescribedData(described_data)
{
	//compute offset and stride
	unsigned int offset = 0;
	m_Stride = 0;
	for (auto& data : m_DescribedData)
	{
		data.m_Offset = offset;
		offset += data.m_Size;
		m_Stride += data.m_Size;
	}
}

VertexBuffer::VertexBuffer(unsigned size)
	:m_Buffer(0)
{
	//just create empty which will be filled later.
	CreateBuffer(nullptr, size);
}

VertexBuffer::VertexBuffer(const void* data, unsigned size)
	:m_Buffer(0)
{
	CreateBuffer(data, size);
}


VertexBuffer::~VertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &m_Buffer);
}

void VertexBuffer::BufferData(const void* data, unsigned size, unsigned offset)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::CreateBuffer(const void* data, unsigned size)
{

	glCreateBuffers(1, &m_Buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}


ElementBuffer::ElementBuffer(const std::vector<unsigned int>& indices)
{
	CreateBuffer(indices.data(), indices.size() * sizeof(unsigned int));
}

ElementBuffer::~ElementBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &m_Buffer);
}

void ElementBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
}

void ElementBuffer::UnBind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementBuffer::CreateBuffer(const void* data, unsigned size)
{
	glGenBuffers(1, &m_Buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

