#include <Mathyw/texture.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

namespace Mathyw {

static constexpr int texture_format(int channels)
{
	switch (channels)
	{
	case 1: return GL_RED;
	case 2: return GL_RG;
	case 3: return GL_RGB;
	case 4: return GL_RGBA;
	default: return -1;
	}
}

Texture::Texture(std::uint8_t const* data, Ivec2 size, int channels)
{
	destruct_this = true;
	this->size = size;
	glGenTextures(1, &textureid);
	glBindTexture(GL_TEXTURE_2D, textureid);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size[0], size[1], 0, texture_format(channels), GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(std::string const& path, int channels)
{
	destruct_this = true;
	glGenTextures(1, &textureid);
	glBindTexture(GL_TEXTURE_2D, textureid);

	stbi_set_flip_vertically_on_load(true);
	int ch;
	stbi_uc* data = stbi_load(path.c_str(), &size[0], &size[1], &ch, channels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size[0], size[1], 0,
		texture_format(channels == 0 || ch < channels ? ch : channels), GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, 0);
	if (data) stbi_image_free(data);
}

Texture::Texture(Texture&& texture) noexcept
	: textureid(texture.textureid), size(texture.size), destruct_this(true)
{
	texture.destruct_this = false;
}

Texture::~Texture()
{
	if (destruct_this)
		glDeleteTextures(1, &textureid);
}

void Texture::Bind(Texture* texture, int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	if (texture) glBindTexture(GL_TEXTURE_2D, texture->textureid);
	else glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind(int slot)
{
	Bind(this, slot);
}

}