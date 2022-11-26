#pragma once
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