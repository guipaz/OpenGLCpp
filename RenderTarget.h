#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"

class RenderTarget
{
public:
	std::unique_ptr<Texture> RenderedTexture;

	RenderTarget(size_t width, size_t height, glm::vec4 clearColor = glm::vec4(0.0f));

	void beginDraw(Shader* shader);
	void endDraw();

private:

	size_t _width, _height;
	glm::vec4 _clearColor;
	std::unique_ptr<Mesh> _mesh;
	GLuint _fbo;
};

