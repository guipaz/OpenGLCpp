#include <iostream>
#include "RenderTarget.h"
#include "Constants.h"
#include "Utils.h"

RenderTarget::RenderTarget(size_t width, size_t height, glm::vec4 clearColor)
{
	_width = width;
	_height = height;
	_clearColor = clearColor;

	RenderedTexture = std::make_unique<Texture>(width, height);

	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RenderedTexture->id, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	_mesh = Utils::createQuad();
}

void RenderTarget::beginDraw(Shader* shader)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glViewport(0, 0, _width, _height);

	glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	shader->setMat4("projectionMatrix", glm::mat4(1.0));
	shader->setMat4("viewMatrix", glm::mat4(1.0));
}

void RenderTarget::endDraw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	RenderedTexture->use();
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
