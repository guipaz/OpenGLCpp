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
#include "Game.h"

int main();
bool configureGlfw();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

GLFWwindow* _window;
std::unique_ptr<Game> _game;
glm::vec4 _clearColor = vec4(0.2f, 0.3f, 0.3f, 1.0f);

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (!configureGlfw())
	{
		std::cout << "Failed to configure GLFW" << std::endl;
		return -1;
	}

	_game = std::make_unique<Game>();
	_game->load();

	float lastTime = 0.0f;
	while (!glfwWindowShouldClose(_window))
	{
		glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float currentTime = (float) glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		_game->update(_window, deltaTime);
		_game->draw(deltaTime);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	_game->unload();

	glfwTerminate();

	return 0;
}

bool configureGlfw()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL Stryfe", NULL, NULL);
	if (_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(_window, mouse_callback);
	glfwSetScrollCallback(_window, scroll_callback);
	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

	return true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	_game->onMouseScrollChanged(xoffset, yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	_game->onMousePositionChanged(xpos, ypos);
}