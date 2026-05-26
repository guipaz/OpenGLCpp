#include "TextureArray.h"
#include "stb_image.h"
#include <iostream>

TextureArray::TextureArray(size_t width, size_t height, const std::vector<std::string>& texturePaths)
{
	stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, _id);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height,
				 texturePaths.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	for (size_t i = 0; i < texturePaths.size(); ++i)
	{
		auto path = texturePaths[i];

		int imageWidth, imageHeight, nrChannels;
		unsigned char* data = stbi_load(path.c_str(), &imageWidth, &imageHeight, &nrChannels, 4);

		if (!data)
		{
			std::cout << "Failed to load texture: " << path << std::endl;
			continue;
		}

		if (width != imageWidth || height != imageHeight)
		{
			std::cout << "Image size is different than the texture array's: " <<
				imageWidth << ";" << imageHeight << "; array "
				<< width << ";" << height << std::endl;
			continue;
		}

		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	setupGlParams();
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

TextureArray::~TextureArray()
{
	glDeleteTextures(1, &_id);
}

void TextureArray::use(unsigned int textureUnit) const
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
}

void TextureArray::setupGlParams()
{
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
