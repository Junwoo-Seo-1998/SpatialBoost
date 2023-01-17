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
#include "Core/Data/Texture.h"
enum class FrameBufferFormat
{
	None = 0,
	RGB,
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
	static std::shared_ptr<FrameBuffer> CreateFrameBuffer(const FrameBufferSpecification& spec);
	FrameBuffer() = delete;
	virtual ~FrameBuffer();

	void Bind() const;
	void UnBind() const;

	void Resize(unsigned int width, unsigned int height);

	unsigned int GetFrameBufferID() const;
	std::shared_ptr<Texture> GetColorTexture(int index=0);
	std::shared_ptr<Texture> GetDepthTexture();

private:
	FrameBuffer(const FrameBufferSpecification& spec);
	void BuildFrameBuffer();
	FrameBufferSpecification m_DescribedFrameBuffer;
	std::vector<FrameBufferFormat> m_ColorFormats;
	FrameBufferFormat m_DepthFormat;

	unsigned int m_FrameBufferID;
	std::vector<std::shared_ptr<Texture>> m_ColorTextures;
	std::shared_ptr<Texture> m_DepthTexture;
};