#version 330 core

in vec2 uvs;
out vec4 FragColor;

uniform sampler2D texture0;

void main()
{
	vec4 texColor = texture(texture0, uvs);
	FragColor = texColor.rgba;
}