#include <iostream>
#include <glad/glad.h>
#include "Texture.h"
#include <GLFW/glfw3.h>
#include "stb_image.h"

Texture::Texture(const std::string& texturePath)
{
	GLenum format = {};
	if (texturePath.ends_with(".png"))
	{
		format = GL_RGBA;
	}
	else
	{
		format = GL_RGB;
	}

	load_texture(texturePath.c_str(), format);
	
	setupGlParams();
	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(size_t width, size_t height)
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	
	setupGlParams();
	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}

void Texture::use(GLenum activeTexture)
{
	glActiveTexture(activeTexture);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::load_texture(const char* textureName, GLenum format)
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(textureName, &width, &height, &nrChannels, 0);

	if (data)
	{
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture: " << textureName << std::endl;
	}

	stbi_image_free(data);
}

void Texture::setupGlParams()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
