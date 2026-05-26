#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

class TextureArray
{
public:
	TextureArray(size_t width, size_t height, const std::vector<std::string>& texturePaths);
	~TextureArray();
	void use(unsigned int textureUnit = 0) const;

private:
	GLuint _id;

	void setupGlParams();
};

