#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: FrameBuffer.h
Purpose: Abstracted class for shader program
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_3
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 10 2022
End Header --------------------------------------------------------*/
#include <vector>
enum class FrameBufferFormat
{
	None = 0,
	RGBA,
	Depth,
};

struct DescribedFrameBufferFormats
{
	DescribedFrameBufferFormats() = default;
	DescribedFrameBufferFormats(std::initializer_list<FrameBufferFormat> formats)
		:Formats(formats) {}

	std::vector<FrameBufferFormat>::iterator begin() { return Formats.begin(); }
	std::vector<FrameBufferFormat>::iterator end() { return Formats.end(); }

	std::vector<FrameBufferFormat> Formats;
};

struct FrameBufferSpecification
{
	unsigned int Width, Height;
	DescribedFrameBufferFormats Formats;
};

class FrameBuffer
{
public:
	FrameBuffer(const FrameBufferSpecification& spec);
	~FrameBuffer();

	void BuildFrameBuffer();

	void Bind() const;
	void Unbind() const;

	void Resize(unsigned int width, unsigned int height);

	unsigned int GetFrameBufferID() const;
	unsigned int GetColorTexture(int index) const;
	unsigned int GetDepthTexture() const;

private:
	FrameBufferSpecification m_DescribedFrameBuffer;
	std::vector<FrameBufferFormat> m_ColorFormats;
	FrameBufferFormat m_DepthFormat;

	unsigned int m_FrameBufferID;
	std::vector<unsigned int> m_ColorBufferIDs;
	unsigned int m_DepthBufferID;
};