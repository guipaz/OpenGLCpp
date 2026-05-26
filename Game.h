#pragma once
#include <memory>
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "GridMap.h"
#include "GameObject.h"
#include "TextureArray.h"

class Game
{
public:
	void draw(float deltaTime);
	void update(GLFWwindow* window, float deltaTime);
	void load();
	void unload();
	void onMousePositionChanged(double x, double y);
	void onMouseScrollChanged(double x, double y);

private:
	std::unique_ptr<Shader> _shader;
	std::unique_ptr<Camera> _camera;
	std::unique_ptr<Texture> _texture;
	std::unique_ptr<Texture> _texture2;

	glm::mat4 _modelMatrix;
	glm::mat4 _viewMatrix;
	glm::mat4 _projectionMatrix;

	glm::mat4 _orthoProjectionMatrix;

	double _lastMouseX = SCREEN_WIDTH / 2;
	double _lastMouseY = SCREEN_HEIGHT / 2;
	bool _isMouseSetup = false;
	float _mouseXoffset;
	float _mouseYoffset;

	std::unique_ptr<Mesh> _cubeMesh;
	std::vector<std::unique_ptr<GameObject>> _gameObjects;

	// map related stuff
	std::unique_ptr<Shader> _mapShader;
	std::unique_ptr<GridMap> _gridMap;
	std::unique_ptr<TextureArray> _textureArray;
	std::vector<unsigned int> _tileMap;
	std::unique_ptr<Texture> _tileMapTexture;

	GLuint _dataTextureId;

	int _lastLeftMouseState;
};

