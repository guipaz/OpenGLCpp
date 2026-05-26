#pragma once
#include "Mesh.h"
#include <memory>

using namespace glm;

class Utils
{
public:
	Utils() = delete;

	static std::unique_ptr<Mesh> CreateCube()
	{
		float _cubeVertices[] = {
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
		};

		float _cubeUvs[] = {
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f
		};

		unsigned int indices[] = {
			 0,  1,  2,  3,  4,  5, // Face 1
			 6,  7,  8,  9, 10, 11, // Face 2
			12, 13, 14, 15, 16, 17, // Face 3
			18, 19, 20, 21, 22, 23, // Face 4
			24, 25, 26, 27, 28, 29, // Face 5
			30, 31, 32, 33, 34, 35  // Face 6
				};

		return std::make_unique<Mesh>(reinterpret_cast<vec3*>(_cubeVertices), std::size(_cubeVertices) / 3,
						 (unsigned int*)indices, sizeof(indices),
						 reinterpret_cast<vec2*>(_cubeUvs));
	}

	static std::unique_ptr<Mesh> createQuad()
	{
		return createQuad(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), glm::vec3(1, 0, 1));
	}

	static std::unique_ptr<Mesh> createQuad(vec3 a, vec3 b, vec3 c, vec3 d)
	{
		glm::vec3 vertices[] = { a, b, c, d };
		unsigned int indices[] = { 0, 1, 2, 1, 3, 2 };
		glm::vec2 uvs[] = { vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1) };

		return std::make_unique<Mesh>(vertices, 4, 
									  indices, 6,
									  uvs);
	};

	static GLuint createDataTexture(const std::vector<unsigned int>& data, size_t width, size_t height)
	{
		GLuint texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, data.data());

		glBindTexture(GL_TEXTURE_2D, 0);

		return texId;
	}
};