#include "FrameBuffer.h"

#include "glad.h"
#include <string>
#include <iostream>
namespace helper
{
	static bool IsDepth(FrameBufferFormat format)
	{
		switch (format)
		{
		case FrameBufferFormat::Depth:
			return true;
		}
		return false;
	}

	static void CreateTextures(unsigned int num_of_textures, std::vector<unsigned int>& textureIDs)
	{
		textureIDs.resize(num_of_textures);
		glCreateTextures(GL_TEXTURE_2D, num_of_textures, textureIDs.data());
	}
	static void CreateTexture(unsigned int* textureID_ptr)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, textureID_ptr);
	}

	static void BindTexture(unsigned int textureID)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	static void DescribeColorTextureBufferAndAttachIt(unsigned int textureID, GLenum innerFormat, GLenum format
		, unsigned int width, unsigned int height, int index)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, innerFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, textureID, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, textureID, 0);
	}


	static void DescribeDepthTextureBufferAndAttachIt(unsigned int textureID, GLenum format, GLenum BufferType
		, unsigned int width, unsigned int height)
	{
		glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, BufferType, GL_TEXTURE_2D, textureID, 0);
	}
}

FrameBuffer::FrameBuffer(const FrameBufferSpecification& spec)
	:m_DescribedFrameBuffer(spec), m_DepthFormat(FrameBufferFormat::None)
	, m_FrameBufferID(0), m_DepthBufferID(0)
{
	if (m_DescribedFrameBuffer.Formats.Formats.empty())
	{
		throw std::string("there is no described Format in FrameBuffer!");
	}

	//todo: not using color format now... might use for docking? 
	for (auto format : m_DescribedFrameBuffer.Formats)
	{
		if (helper::IsDepth(format))
		{
			m_DepthFormat = format;
		}
		else
		{
			m_ColorFormats.emplace_back(format);
		}
	}

	BuildFrameBuffer();
}

FrameBuffer::~FrameBuffer()
{
	Unbind();
	glDeleteFramebuffers(1, &m_FrameBufferID);
	glDeleteTextures(static_cast<unsigned int>(m_ColorBufferIDs.size()), m_ColorBufferIDs.data());
	glDeleteTextures(1, &m_DepthBufferID);
}

void FrameBuffer::BuildFrameBuffer()
{
	if (m_FrameBufferID != 0)
	{
		//remove previous data
		glDeleteFramebuffers(GL_FRAMEBUFFER, &m_FrameBufferID);
		glDeleteTextures(static_cast<unsigned int>(m_ColorBufferIDs.size()), m_ColorBufferIDs.data());
		glDeleteTextures(1, &m_DepthBufferID);

		m_ColorBufferIDs.clear();
		m_DepthBufferID = 0;
	}

	glCreateFramebuffers(1, &m_FrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

	//if there is color formats then make color buffer... making this i might use it for imgui stuff
	if (m_ColorFormats.empty() == false)
	{
		const unsigned num_of_textures = static_cast<unsigned int>(m_ColorFormats.size());
		helper::CreateTextures(num_of_textures, m_ColorBufferIDs);

		const unsigned int width = m_DescribedFrameBuffer.Width;
		const unsigned int height = m_DescribedFrameBuffer.Height;
		for (unsigned int i = 0; i < num_of_textures; ++i)
		{
			const FrameBufferFormat format = m_ColorFormats[i];
			helper::BindTexture(m_ColorBufferIDs[i]);
			switch (format)
			{
			case  FrameBufferFormat::RGBA:
			{
				helper::DescribeColorTextureBufferAndAttachIt(m_ColorBufferIDs[i], GL_RGBA8, GL_RGBA, width, height, i);
				break;
			}
			}
		}
	}

	//make depth
	if (m_DepthFormat != FrameBufferFormat::None)
	{
		helper::CreateTexture(&m_DepthBufferID);
		helper::BindTexture(m_DepthBufferID);
		const unsigned int width = m_DescribedFrameBuffer.Width;
		const unsigned int height = m_DescribedFrameBuffer.Height;
		//might add stencil buffer?
		switch (m_DepthFormat)
		{
		case FrameBufferFormat::Depth:
			helper::DescribeDepthTextureBufferAndAttachIt(m_DepthBufferID, GL_DEPTH_COMPONENT24, GL_DEPTH_ATTACHMENT, width, height);
			break;
		}
	}
	if (m_ColorFormats.size() > 1)
	{
		const unsigned int size = static_cast<unsigned int>(m_ColorFormats.size());
		if (size > 4)
		{
			throw std::string("max color attachment size is 4");
		}

		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(size, buffers);
	}
	else if (m_ColorFormats.empty())//means only depth
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Couldn't make FrameBuffer!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
	glViewport(0, 0, m_DescribedFrameBuffer.Width, m_DescribedFrameBuffer.Height);
	glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Resize(unsigned width, unsigned height)
{
	//ignore to set size as zero
	if (width == 0 || height == 0)
	{
		return;
	}
	m_DescribedFrameBuffer.Width = width;
	m_DescribedFrameBuffer.Height = height;
	BuildFrameBuffer();
}

unsigned FrameBuffer::GetFrameBufferID() const
{
	return m_FrameBufferID;
}

unsigned FrameBuffer::GetColorTexture(int index) const
{
	return m_ColorBufferIDs[index];
}

unsigned FrameBuffer::GetDepthTexture() const
{
	return m_DepthBufferID;
}