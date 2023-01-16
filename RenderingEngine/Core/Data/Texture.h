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
#include <memory>
#include <tuple>
struct TextureData;
class Texture
{
public:
	static std::shared_ptr<Texture> CreateTexture(std::shared_ptr<TextureData> texture_data);
	static std::shared_ptr<Texture> CreateTexture(const TextureData& texture_data);
	virtual ~Texture();
	std::tuple<int, int> GetWidthHeight() const;
	unsigned int GetTextureID() const;

	void BindTexture(unsigned int unit = 0);
private:
	Texture(std::shared_ptr<TextureData> texture_data);
	Texture(const TextureData& texture_data);
	int m_Width;
	int m_Height;
	unsigned int m_TextureID;
};