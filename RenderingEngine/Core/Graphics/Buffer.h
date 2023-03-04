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
#include<memory>
#include"Core/Utils/DataType.h"

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
	unsigned ShaderDataTypeToOpenGLBaseType() const;
};

class DescribedData
{
public:
	DescribedData() :m_Stride(0) {}
	DescribedData(const std::initializer_list<DataAndLayoutLocation>& described_data);

	unsigned int GetStride() const { return m_Stride; }

	[[nodiscard]] int GetSize() const { return static_cast<int>(m_DescribedData.size()); }
	[[nodiscard]] std::vector<DataAndLayoutLocation>::iterator begin() { return m_DescribedData.begin(); }
	[[nodiscard]] std::vector<DataAndLayoutLocation>::iterator end() { return m_DescribedData.end(); }
	[[nodiscard]] std::vector<DataAndLayoutLocation>::const_iterator begin() const { return m_DescribedData.cbegin(); }
	[[nodiscard]] std::vector<DataAndLayoutLocation>::const_iterator end() const { return m_DescribedData.cend(); }
private:
	std::vector<DataAndLayoutLocation> m_DescribedData;
	unsigned int m_Stride;
};

class VertexBuffer
{
public:
	VertexBuffer() = delete;
	~VertexBuffer();
	static std::shared_ptr<VertexBuffer> CreateVertexBuffer(int byte_size);
	void Bind() const;
	void BindToVertexArray() const;
	void UnBind() const;
	void SetData(int size, const void* data, unsigned offset = 0);
	void SetDataTypes(const DescribedData& data);
private:
	VertexBuffer(int byte_size);
	void CreateBuffer(int size, const void* data);
	unsigned m_Buffer;
	DescribedData m_DescribedData;
};

class IndexBuffer
{
public:
	IndexBuffer() = delete;
	virtual ~IndexBuffer();
	static std::shared_ptr<IndexBuffer> CreateIndexBuffer(int byte_size);
	void Bind() const;
	void BindToVertexArray() const;
	void UnBind() const;
	void SetData(int size, const void* data, unsigned offset = 0);
private:
	IndexBuffer(int byte_size);
	void CreateBuffer(unsigned size, const void* data);

	unsigned m_Buffer;
};