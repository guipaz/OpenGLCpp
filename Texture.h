#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Texture
{
public:
	GLuint ID;
	Texture(size_t width, size_t height);
	Texture(const std::string& texturePath);
	~Texture();
	void use(GLenum activeTexture = GL_TEXTURE0);

private:
	void load_texture(const char* texturePath, GLenum format);
	void setupGlParams();
};

