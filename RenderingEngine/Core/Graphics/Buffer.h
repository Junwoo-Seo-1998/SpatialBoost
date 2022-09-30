#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Buffer.h
Purpose: Abstracted classes for vertex buffer object and index buffer with helper classes
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 10 2022
End Header --------------------------------------------------------*/
#include<vector>
#include<glad.h>

enum DataType
{
	None = 0, Bool, Int, Int2, Int3, Int4, Float, Float2, Float3, Float4, Mat3, Mat4,
};

struct DataAndLayoutLocation
{
	DataType m_DataType;
	unsigned int m_LayoutLocation;
	unsigned int m_Size;
	unsigned int m_ElementCount;
	unsigned int m_Offset;
	bool m_Normalize;

	DataAndLayoutLocation() = delete;
	DataAndLayoutLocation(unsigned int layout_location, DataType data, bool normalize = false);
	GLenum ShaderDataTypeToOpenGLBaseType() const;
};

class DescribedData
{
public:
	DescribedData():m_Stride(0) {}
	DescribedData(const std::initializer_list<DataAndLayoutLocation>& described_data);

	unsigned int GetStride() const { return m_Stride; }

	std::vector<DataAndLayoutLocation>::iterator begin() { return m_DescribedData.begin(); }
	std::vector<DataAndLayoutLocation>::iterator end() { return m_DescribedData.end(); }
	std::vector<DataAndLayoutLocation>::const_iterator begin() const { return m_DescribedData.cbegin(); }
	std::vector<DataAndLayoutLocation>::const_iterator end() const { return m_DescribedData.cend(); }
private:
	std::vector<DataAndLayoutLocation> m_DescribedData;
	unsigned int m_Stride;
};

class VertexBuffer
{
public:
	VertexBuffer(unsigned int size);
	VertexBuffer(const void* data, unsigned size);
	virtual ~VertexBuffer();

	void BufferData(const void* data, unsigned size, unsigned offset = 0);

	void Bind() const;
	void Unbind() const;

	void DescribeData(const DescribedData& described_Data) { m_DescribedData = described_Data; }
	const DescribedData& GetDescribedData() const { return m_DescribedData; }
private:
	void CreateBuffer(const void* data, unsigned size);
private:
	GLuint m_Buffer;
	DescribedData m_DescribedData;
};



class ElementBuffer
{
public:
	ElementBuffer(const std::vector<unsigned int>& indices);
	virtual ~ElementBuffer();
	void Bind() const;
	void UnBind() const;
private:
	void CreateBuffer(const void* data, unsigned size);
private:
	GLuint m_Buffer;
};