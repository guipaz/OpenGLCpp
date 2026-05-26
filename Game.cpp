#include "Game.h"
#include "Utils.h"

void Game::load()
{
	_shader = std::make_unique<Shader>("basic_vertex.vert", "basic_fragment.frag");
	_camera = std::make_unique<Camera>();
	_cubeMesh = std::move(Utils::CreateCube());

	// loads and creates texture
	_texture = std::make_unique<Texture>("container.jpg");
	_texture2 = std::make_unique<Texture>("awesomeface.png");

	// loads map related stuff
	_gridMap = std::make_unique<GridMap>(100, 100, 0.125f, 0.0625f);
	_mapShader = std::make_unique<Shader>("map.vert", "map.frag");
	_textureArray = std::make_unique<TextureArray>(256, 256,
												   std::vector<std::string> { "textures/grass.png",
												   "textures/dirt.png", "textures/pavement.png" });
	_tileMap = std::vector<unsigned int>(_gridMap->columns * _gridMap->rows);
	for (size_t y = 0; y < _gridMap->rows; ++y)
	{
		for (size_t x = 0; x < _gridMap->columns; ++x)
		{
			unsigned int tileId = 0;
			if (y == 0 || x == 0)
			{
				tileId = 1;
			}
			else if (y == 2 || x == 2)
			{
				tileId = 2;
			}

			//DEBUG
			_tileMap[y * _gridMap->rows + x] = tileId; // DEBUG
		}
	}

	_dataTextureId = Utils::createDataTexture(_tileMap, _gridMap->columns, _gridMap->rows);
	//_tileMapTexture = std::make_unique<Texture>((GLuint)dataTextureId);

	// setups camera matrices
	_modelMatrix = glm::mat4(1.0f);
	_modelMatrix = glm::rotate(_modelMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	_viewMatrix = glm::mat4(1.0f);
	_viewMatrix = glm::translate(_viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

	_camera->Position.y = 10;

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, 5.0f),
		glm::vec3(3.0f, 2.0f, 5.0f),
	};

	glm::vec3 rotationAxis = glm::vec3(1.0f, 0.3f, 0.5f);
	for (int i = 0; i <= cubePositions->length(); i++)
	{
		float angle = 20.0f * i;

		auto gameObject = std::make_unique<GameObject>();
		gameObject->position = cubePositions[i] + vec3(_gridMap->cellSize);
		gameObject->rotation = glm::angleAxis(glm::radians(angle), glm::normalize(rotationAxis));
		gameObject->mesh = _cubeMesh.get();
		gameObject->texture = _texture.get();

		//_gameObjects.push_back(std::move(gameObject));
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

	_mouseXoffset = (float)(x - _lastMouseX);
	_mouseYoffset = (float)(y - _lastMouseY);
	_lastMouseX = x;
	_lastMouseY = y;
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
	_camera->applyMatrices(_shader.get());

	// activate texture
	//_texture->use();

	// draw objects
	for (const auto& gameObject : _gameObjects)
	{
		gameObject->draw(_shader.get());
	}





	// renders map related stuff
	_mapShader->use();

	_mapShader->setInt("textureArray", 0);
	_mapShader->setInt("tileMapTexture", 1);
	_mapShader->setFloat("cellSize", _gridMap->cellSize);

	_textureArray->use(0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _dataTextureId);
	//_tileMapTexture->use(1);
	
	_camera->applyMatrices(_mapShader.get());
	_gridMap->draw(_mapShader.get());


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

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		_camera->ProcessMouseMovement(_mouseXoffset, _mouseYoffset, true);
	}

	int currentLeftMouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (_lastLeftMouseState == GLFW_RELEASE && currentLeftMouseState == GLFW_PRESS)
	{
		std::cout << "Mouse click" << std::endl;
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		Ray ray = _camera->raycastFromScreenPosition((float)xpos, (float)ypos);
		std::cout << "Ray - X: " << ray.direction.x << "; Y: " << ray.direction.y << std::endl;
	}

	_lastLeftMouseState = currentLeftMouseState;
	_mouseXoffset = 0;
	_mouseYoffset = 0;
}