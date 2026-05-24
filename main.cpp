// MEMORY DEBUGGING
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

#include <cstddef>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "GridMap.h"
#include "MeshUtils.h"
#include "Constants.h"
#include <memory>


int main();

void load_scene();
void unloadScene();

void update(float deltaTime);
void draw(float deltaTime);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

std::unique_ptr<Shader> _shader;
std::unique_ptr<Camera> _camera;
std::unique_ptr<Texture> _texture;
std::unique_ptr<Texture> _texture2;

GLFWwindow* _window;

glm::vec4 _clearColor = vec4(0.2f, 0.3f, 0.3f, 1.0f);

glm::mat4 _modelMatrix;
glm::mat4 _viewMatrix;
glm::mat4 _projectionMatrix;

glm::mat4 _orthoProjectionMatrix;

double _lastMouseX = SCREEN_WIDTH / 2;
double _lastMouseY = SCREEN_HEIGHT / 2;
bool _isMouseSetup = false;

std::unique_ptr<Mesh> _cubeMesh;
std::vector<std::unique_ptr<GameObject>> _gameObjects;

std::unique_ptr<GridMap> _gridMap;

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

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL Stryfe", NULL, NULL);
	if (_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(_window, mouse_callback);
	glfwSetScrollCallback(_window, scroll_callback);
	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

	load_scene();

	float lastTime = 0.0f;
	while (!glfwWindowShouldClose(_window))
	{
		float currentTime = (float) glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		update(deltaTime);

		draw(deltaTime);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	unloadScene();

	glfwTerminate();

	return 0;
}

void unloadScene()
{
	_gameObjects.clear();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	_camera->ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!_isMouseSetup)
	{
		_isMouseSetup = true;
		_lastMouseX = xpos;
		_lastMouseY = ypos;
	}

	float xoffset = xpos - _lastMouseX;
	float yoffset = ypos - _lastMouseY;
	_lastMouseX = xpos;
	_lastMouseY = ypos;

	_camera->ProcessMouseMovement(xoffset, yoffset, true);
}

void update(float deltaTime)
{
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(_window, true);
	}

	const float cameraSpeed = 0.05f;
	if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		_camera->ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	}
	if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		_camera->ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		_camera->ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	}
	if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		_camera->ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	}
	if (glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		_camera->ProcessKeyboard(Camera_Movement::DOWN, deltaTime);
	}
	if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS)
	{
		_camera->ProcessKeyboard(Camera_Movement::UP, deltaTime);
	}
}

void load_scene()
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

void draw(float deltaTime)
{
	// clear screen
	glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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