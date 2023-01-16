#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_2
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include<memory>
enum class TextureChannel
{
	RGB,
	RGBA,
	Depth,
	None,
};

struct TextureChannelData
{
	unsigned TextureChannelTypeToOpenGLInnerType() const;
	unsigned TextureChannelTypeToOpenGLType() const;
	TextureChannelData(TextureChannel channel) :channel(channel) {}
	TextureChannel channel;
};

struct TextureData
{
	TextureData() :width(0), height(0), data(nullptr), channel(TextureChannel::RGBA) {}
	TextureData(int width, int height,
		std::shared_ptr<unsigned char[]> data = nullptr, TextureChannelData channel = TextureChannel::RGBA)
		:width(width), height(height), channel(channel), data(data) {}
	int width = 0;
	int height = 0;
	std::shared_ptr<unsigned char[]> data{};
	TextureChannelData channel = TextureChannel::RGBA;
};
