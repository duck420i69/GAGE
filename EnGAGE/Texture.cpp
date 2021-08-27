#include "pch.h"
#include "Texture.h"

#include "Logger.h"

#include <glad/glad.h>
#include <stb_image.h>


Texture::Texture(const std::string& file_path) noexcept :
	mName(file_path), mID(0), mWidth(0), mHeight(0)
{
	try
	{
		int width = 0, height = 0, bpp = 0;
		stbi_uc* image_data = nullptr;
		stbi_set_flip_vertically_on_load(true);
		image_data = stbi_load(file_path.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
		if (!image_data)
		{
			std::stringstream ss;
			ss << "Failed to load image : " << file_path << ", stb_image's report: " <<
				std::string(stbi_failure_reason());

			throw std::runtime_error(ss.str());
		}
		mWidth = width;
		mHeight = height;
		glGenTextures(1, &mID);
		glBindTexture(GL_TEXTURE_2D, mID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, width, height, 0, GL_RGBA,GL_UNSIGNED_BYTE, image_data);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(image_data);

		Logger::info("Creating texture: {}", file_path);
	}
	catch (std::exception& e)
	{
		Logger::error("Texture exception thrown: {}", e.what());
	}
}

Texture::~Texture() noexcept
{
	Logger::info("Destroying texture: {}", mID);
	glDeleteTextures(1, &mID);
}

void Texture::Bind(int slot) const noexcept
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, mID);
}
