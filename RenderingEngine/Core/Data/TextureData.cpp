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
	case TextureChannel::Depth:
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
	case TextureChannel::Depth:
		return GL_DEPTH_STENCIL;
	default:
		break;
	}
	//ENGINE_ASSERT(false, "Not Supported Texture Channel");
	return 0;
}
