#include "Game.h"
#include "MeshUtils.h"

void Game::load()
{
	_shader = std::make_unique<Shader>("basic_vertex.vert", "basic_fragment.frag");
	_camera = std::make_unique<Camera>();

	_gridMap = std::make_unique<GridMap>(10, 10, 1, 0.25f);

	_cubeMesh = std::move(MeshUtils::CreateCube());

	// loads and creates texture
	_texture = std::make_unique<Texture>("container.jpg");
	_texture2 = std::make_unique<Texture>("awesomeface.png");

	// setups camera matrices
	_modelMatrix = glm::mat4(1.0f);
	_modelMatrix = glm::rotate(_modelMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	_viewMatrix = glm::mat4(1.0f);
	_viewMatrix = glm::translate(_viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

	_camera->Position.y = 10;

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, 15.0f),
		glm::vec3(1.5f, 2.2f, 2.5f),
		glm::vec3(3.8f, 2.0f, 12.3f),
		glm::vec3(2.4f, 0.4f, 3.5f),
		glm::vec3(1.7f,  3.0f, 7.5f),
		glm::vec3(1.3f, 2.0f, 2.5f),
		glm::vec3(1.5f,  2.0f, 2.5f),
		glm::vec3(1.5f,  0.2f, 1.5f),
		glm::vec3(1.3f,  1.0f, 1.5f)
	};

	glm::vec3 rotationAxis = glm::vec3(1.0f, 0.3f, 0.5f);
	for (unsigned int i = 0; i < 10; i++)
	{
		float angle = 20.0f * i;

		auto gameObject = std::make_unique<GameObject>();
		gameObject->position = cubePositions[i];
		gameObject->rotation = glm::angleAxis(glm::radians(angle), glm::normalize(rotationAxis));
		gameObject->mesh = _cubeMesh.get();

		_gameObjects.push_back(std::move(gameObject));
	}
}

void Game::unload()
{
	_gameObjects.clear();
}

void Game::onMousePositionChanged(double x, double y)
{
	if (!_isMouseSetup)
	{
		_isMouseSetup = true;
		_lastMouseX = x;
		_lastMouseY = y;
	}

	float xoffset = (float)(x - _lastMouseX);
	float yoffset = (float)(y - _lastMouseY);
	_lastMouseX = x;
	_lastMouseY = y;

	_camera->ProcessMouseMovement(xoffset, yoffset, true);
}

void Game::onMouseScrollChanged(double x, double y)
{
	_camera->ProcessMouseScroll((float)y);
}

void Game::draw(float deltaTime)
{
	// draw world
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// activate shader
	_shader->use();
	_shader->setInt("texture0", 0);

	// activate camera
	_camera->use(_shader.get());

	// activate texture
	_texture->use();

	// draw objects
	_gridMap->draw(_shader.get());
	for (const auto& gameObject : _gameObjects)
	{
		gameObject->draw(_shader.get());
	}

	// draw ui
	//TODO: change to an ui camera
	_shader->setMat4("projectionMatrix", glm::mat4(1.0));
	_shader->setMat4("viewMatrix", glm::mat4(1.0));
	_shader->setMat4("modelMatrix",
					 glm::translate(
						 glm::rotate(
							 glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
						 vec3(-.5f, 0.0f, -.5f)));

	//TODO: UI elements
}

void Game::update(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	const float cameraSpeed = 0.05f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		_camera->ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		_camera->ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		_camera->ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		_camera->ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		_camera->ProcessKeyboard(Camera_Movement::DOWN, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		_camera->ProcessKeyboard(Camera_Movement::UP, deltaTime);
	}
}