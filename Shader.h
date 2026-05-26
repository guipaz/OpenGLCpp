#pragma once

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setIntArray(const std::string& name, std::vector<int> value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
	void setVec2(const std::string& name, glm::vec2 value) const;

private:
	unsigned int compile_shader_source(const std::string& filePath, GLenum shaderType);
	unsigned int linkShader(unsigned int vertexShader, unsigned int fragmentShader);
	std::string loadFileContents(const std::string& filePath);
};