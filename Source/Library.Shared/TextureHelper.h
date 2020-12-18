#pragma once

namespace Library
{
	class TextureHelper final
	{
		TextureHelper() = delete;
		TextureHelper(const TextureHelper&) = delete;
		TextureHelper& operator=(const TextureHelper&) = delete;
		TextureHelper(TextureHelper&&) = delete;
		TextureHelper& operator=(TextureHelper&&) = delete;
		~TextureHelper() = default;

	public:
		// Create new texture ID and load new 2D texture with the given wrapping and filtering mode
		static GLuint LoadTexture(const char* fileName, GLint wrapping, GLint filtering);

		// Create new texture ID and load cube map texture with the given wrapping and filtering mode
		static GLuint LoadCubeMap(const std::vector<const char*>& faces, GLint wrapping, GLint filtering);

		static GLuint LoadCubeMap(const char* posX, const char* negX, const char* posY, const char* negY, const char* posZ, const char* negZ, GLint wrapping, GLint filtering);

	};
}
