#version 330 core

in vec2 uvs;
in vec3 fragPos;
out vec4 fragColor;

uniform float cellSize;

uniform sampler2DArray textureArray;
uniform usampler2D tileMapTexture;

void main()
{
	// cell textures
	vec2 gridUV = fragPos.xz / cellSize;

	ivec2 cellIndex = ivec2(floor(gridUV));
	uint textureLayer = texelFetch(tileMapTexture, cellIndex, 0).r;

	vec4 texColor = texture(textureArray, vec3(uvs, float(textureLayer)));

	// grid
	float thickness = 0.008;
	float fade = 0.005;
	vec2 cellUV = fract(gridUV);

	// smoothstep(edge0, edge1, x)
	// edge0: lower threshold (smoothly increase from 0)
	// edge1: upper threshold (firmly reaches 1)
	float lineX = smoothstep(thickness + fade,			thickness,			cellUV.x) +
				  smoothstep(1.0 - thickness - fade,	1.0 - thickness,	cellUV.x);

	float lineY = smoothstep(thickness + fade,			thickness,			cellUV.y) +
				  smoothstep(1.0 - thickness - fade,	1.0 - thickness,	cellUV.y);
	
	float gridFactor = clamp(lineX + lineY, 0.0, 1.0);
	vec4 gridColor = vec4(1.0, 1.0, 1.0, 0.25);

	// output
	fragColor = mix(texColor, vec4(gridColor.rgb, 1.0), gridFactor * gridColor.a);
	//fragColor = texColor;
	//fragColor = vec4(cellUV.x * 0.2, cellUV.y * 0.1, 1.0, 1.0);
}