#pragma once
#include <memory>
#include <tuple>
struct TextureData;
class Texture
{
public:
	Texture(std::shared_ptr<TextureData> texture_data);
	virtual ~Texture();
	std::tuple<int, int> GetWidthHeight() const;
	unsigned int GetTextureID() const;
	void Bind(unsigned int unit = 0);
private:
	int m_Width;
	int m_Height;
	unsigned int m_TextureID;
};