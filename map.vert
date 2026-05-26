#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 uvs;
out vec3 fragPos;

void main()
{
	uvs = aTexCoords;
	fragPos = aPos;

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
}