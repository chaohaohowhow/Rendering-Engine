#include "pch.h"
#include "TextureHelper.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Library
{
	GLuint TextureHelper::LoadTextureRGB(const char* fileName, GLint wrapping, GLint filtering)
	{
		return LoadTextureHelper(fileName, GL_TEXTURE_2D, wrapping, filtering, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
	}

	GLuint TextureHelper::LoadTextureRGBA(const char* fileName, GLint wrapping, GLint filtering)
	{
		return LoadTextureHelper(fileName, GL_TEXTURE_2D, wrapping, filtering, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	}

	GLuint TextureHelper::LoadCubeMap(const std::vector<const char*>& faces, GLint wrapping, GLint filtering)
	{
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		int width, height, nrChannel;
		for (size_t i = 0; i < faces.size(); ++i)
		{
			unsigned char* textureData = stbi_load(faces[i], &width, &height, &nrChannel, 0);
			if (textureData)
			{
				glTexImage2D(static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i), 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
				stbi_image_free(textureData);
			}
			else
			{
				stbi_image_free(textureData);
				throw std::runtime_error("stbi_load() failed!");
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filtering);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filtering);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapping);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapping);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapping);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		return texture;
	}

	GLuint TextureHelper::LoadCubeMap(const char* posX, const char* negX, const char* posY, const char* negY, const char* posZ, const char* negZ, GLint wrapping, GLint filtering)
	{
		const std::vector<const char*> faces = {
			posX,
			negX,
			posY,
			negY,
			posZ,
			negZ
		};

		return LoadCubeMap(faces, wrapping, filtering);
	}

	GLuint TextureHelper::LoadTextureHelper(const char* fileName, GLenum bindTarget, GLint wrapping, GLint filtering, GLint internalFormat, GLenum format, GLenum type)
	{
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(bindTarget, texture);
		glTexParameteri(bindTarget, GL_TEXTURE_WRAP_S, wrapping);
		glTexParameteri(bindTarget, GL_TEXTURE_WRAP_T, wrapping);
		glTexParameteri(bindTarget, GL_TEXTURE_MIN_FILTER, filtering);
		glTexParameteri(bindTarget, GL_TEXTURE_MAG_FILTER, filtering);

		int width, height, nrChannel;
		unsigned char* textureData = stbi_load(fileName, &width, &height, &nrChannel, 0);
		if (textureData)
		{
			glTexImage2D(bindTarget, 0, internalFormat, width, height, 0, format, type, textureData);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(textureData);
		}
		else
		{
			stbi_image_free(textureData);
			throw std::runtime_error("stbi_load() failed!");
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		return texture;
	}

}