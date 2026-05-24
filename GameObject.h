#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class GameObject
{
public:
	unsigned int ID;

	glm::vec3 position;
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	Mesh* mesh;

	virtual void draw(Shader* shader)
	{
		glm::mat4 modelMatrix = glm::mat4(1.0);
		modelMatrix = glm::translate(modelMatrix, position);	// position
		modelMatrix = modelMatrix * glm::mat4_cast(rotation);	// rotation
		modelMatrix = glm::scale(modelMatrix, scale);			// scale

		shader->setMat4("modelMatrix", modelMatrix);

		mesh->draw();
	}

private:

};