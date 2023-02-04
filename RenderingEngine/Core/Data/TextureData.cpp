/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs350_1
Author: Junwoo Seo, junwoo.seo, 0055213
End Header --------------------------------------------------------*/
#include "TextureData.h"
#include "glad.h"

unsigned TextureChannelData::TextureChannelTypeToOpenGLInnerType() const
{
	switch (channel)
	{
	case TextureChannel::RGB:
		return GL_RGB8;
	case TextureChannel::RGBA:
		return GL_RGBA8;
	case TextureChannel::RGBA32F:
		return GL_RGBA32F;
	case TextureChannel::Depth:
		return GL_DEPTH_COMPONENT24;
	case TextureChannel::DepthStencil:
		return GL_DEPTH24_STENCIL8;
	default:
		break;
	}
	//ENGINE_ASSERT(false, "Not Supported Texture Channel");
	return 0;
}

unsigned TextureChannelData::TextureChannelTypeToOpenGLType() const
{
	switch (channel)
	{
	case TextureChannel::RGB:
		return GL_RGB;
	case TextureChannel::RGBA:
		return GL_RGBA;
	case TextureChannel::RGBA32F:
		return GL_RGBA32F;
	case TextureChannel::Depth:
		return GL_DEPTH;
	case TextureChannel::DepthStencil:
		return GL_DEPTH_STENCIL;
	default:
		break;
	}
	//ENGINE_ASSERT(false, "Not Supported Texture Channel");
	return 0;
}
