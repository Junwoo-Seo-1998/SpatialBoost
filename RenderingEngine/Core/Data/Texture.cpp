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
#include "Texture.h"
#include "TextureData.h"
#include <glad.h>

std::shared_ptr<Texture> Texture::CreateTexture(std::shared_ptr<TextureData> texture_data)
{
	return std::shared_ptr<Texture>(new Texture{ texture_data });
}

std::shared_ptr<Texture> Texture::CreateTexture(const TextureData& texture_data)
{
	return std::shared_ptr<Texture>(new Texture{ texture_data });
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_TextureID);
}

std::tuple<int, int> Texture::GetWidthHeight() const
{
	return { m_Width, m_Height };
}

unsigned int Texture::GetTextureID() const
{
	return m_TextureID;
}

void Texture::BindTexture(unsigned int unit)
{
	glBindTextureUnit(unit, m_TextureID);
}

Texture::Texture(std::shared_ptr<TextureData> texture_data)
	:m_Width(texture_data->width), m_Height(texture_data->height)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

	glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureStorage2D(m_TextureID, 1, texture_data->channel.TextureChannelTypeToOpenGLInnerType(), m_Width, m_Height);

	if (texture_data->data != nullptr)
	{
		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, texture_data->channel.TextureChannelTypeToOpenGLType(), GL_UNSIGNED_BYTE, texture_data->data.get());
		glGenerateTextureMipmap(m_TextureID);
	}
}

Texture::Texture(const TextureData& texture_data)
	:m_Width(texture_data.width), m_Height(texture_data.height)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

	glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureStorage2D(m_TextureID, 1, texture_data.channel.TextureChannelTypeToOpenGLInnerType(), m_Width, m_Height);

	if (texture_data.data != nullptr)
	{
		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, texture_data.channel.TextureChannelTypeToOpenGLType(), GL_UNSIGNED_BYTE, texture_data.data.get());
		glGenerateTextureMipmap(m_TextureID);
	}
}


