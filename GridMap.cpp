#include "GridMap.h"
#include <random>
#include <iostream>
#include "RenderTarget.h"
#include "Constants.h"

using namespace glm;

GridMap::GridMap(unsigned int columns, unsigned int rows, unsigned int cellSize, float heightStep)
{
	if (!_mapGenerationShader)
	{
		_mapGenerationShader = std::make_unique<Shader>("", "");
	}

	Columns = columns;
	Rows = rows;
	CellSize = cellSize;

	//std::random_device rd;
	std::mt19937 gen(1);

	_cells = std::vector<GridMapCell>(Columns * Rows);
	for (size_t z = 0; z < Rows; ++z)
	{
		for (size_t x = 0; x < Columns; ++x)
		{
			// generate a random y
			std::uniform_int_distribution distr(0, 2);
			float random_y = distr(gen) * heightStep;

			_cells[z * Columns + x] = GridMapCell { (unsigned int) x, random_y,
				(unsigned int) z, 0, 0, 0, 0 };
		}
	}

	// DEBUGGING
	_cells[2 * Columns + 2].smoothEdge = false;
	_cells[3 * Columns + 2].smoothEdge = false;
	_cells[2 * Columns + 3].smoothEdge = false;
	_cells[3 * Columns + 3].smoothEdge = false;

	_cells[2 * Columns + 2].Y = .5f;
	_cells[3 * Columns + 2].Y = .5f;
	_cells[2 * Columns + 3].Y = .5f;
	_cells[3 * Columns + 3].Y = .5f;

	// edge smoothing
	int iWidth = static_cast<int>(Columns);
	int iHeight = static_cast<int>(Rows);
	for (int z = -1; z < iHeight; ++z)
	{
		for (int x = -1; x < iWidth; ++x)
		{
			bool isN = z >= 0, isS = z < iHeight - 1, isW = x >= 0, isE = x < iWidth - 1;
			int iNw = isN && isW ? (z * iWidth + x) : -1,
				iNe = isN && isE ? (z * iWidth + (x + 1)) : -1,
				iSw = isS && isW ? ((z + 1) * iWidth + x) : -1,
				iSe = isS && isE ? ((z + 1) * iWidth + (x + 1)) : -1;
			
			// only smooths and consider for median the cells that are toggled for it
			bool applyNw = iNw >= 0 && _cells[iNw].smoothEdge;
			bool applyNe = iNe >= 0 && _cells[iNe].smoothEdge;
			bool applySw = iSw >= 0 && _cells[iSw].smoothEdge;
			bool applySe = iSe >= 0 && _cells[iSe].smoothEdge;

			// calculates the median and apply the difference to each cells mod
			float median_y = 0;
			unsigned int iMedian = 0;
			if (applyNw)
			{
				++iMedian;
				median_y += _cells[iNw].Y;
			}
			if (applyNe)
			{
				++iMedian;
				median_y += _cells[iNe].Y;
			}
			if (applySw)
			{
				++iMedian;
				median_y += _cells[iSw].Y;
			}
			if (applySe)
			{
				++iMedian;
				median_y += _cells[iSe].Y;
			}
			median_y /= iMedian;

			if (applyNw) _cells[iNw].seModY = median_y - _cells[iNw].Y;
			if (applyNe) _cells[iNe].swModY = median_y - _cells[iNe].Y;
			if (applySw) _cells[iSw].neModY = median_y - _cells[iSw].Y;
			if (applySe) _cells[iSe].nwModY = median_y - _cells[iSe].Y;
		}
	}

	generateMesh();
	mesh = _generatedMesh.get();
}

void GridMap::generateMesh()
{
	//TODO: calculate option for single-vertex edged tiles
	// vertices, indices, uvs
	const size_t vertexCount = Columns * Rows * 4;
	const size_t indexCount = Columns * Rows * 6;

	vec3* vertices = new glm::vec3[Columns * Rows * 4];
	unsigned int* indices = new unsigned int[Columns * Rows * 2 * 3]; // * 2 triangles * 3 vertices per triangle
	vec2* uvs = new glm::vec2[Columns * Rows * 4];

	unsigned int i = 0;
	unsigned int j = 0;
	
	float uvStepX = 1.0f / Columns;
	float uvStepY = 1.0f / Rows;

	for (size_t y = 0; y < Rows; ++y)
	{
		for (size_t x = 0; x < Columns; ++x)
		{
			GridMapCell tile = _cells[y * Columns + x];

			// set the 6 indices for this tile's 2 triangles
			indices[j++] = i;
			indices[j++] = i + 1;
			indices[j++] = i + 2;
			indices[j++] = i + 1;
			indices[j++] = i + 2;
			indices[j++] = i + 3;

			uvs[i] =		vec2(tile.X			* uvStepX, tile.Z		* uvStepY);
			uvs[i + 1] =	vec2((tile.X + 1)	* uvStepX, tile.Z		* uvStepY);
			uvs[i + 2] =	vec2(tile.X			* uvStepX, (tile.Z + 1) * uvStepY);
			uvs[i + 3] =	vec2((tile.X + 1)	* uvStepX, (tile.Z + 1) * uvStepY);

			// set the 4 vertices for this tile
			vertices[i++] = vec3(tile.X * CellSize,				tile.Y + tile.nwModY, tile.Z * CellSize);
			vertices[i++] = vec3(tile.X * CellSize + CellSize,	tile.Y + tile.neModY, tile.Z * CellSize);
			vertices[i++] = vec3(tile.X * CellSize,				tile.Y + tile.swModY, tile.Z * CellSize + CellSize);
			vertices[i++] = vec3(tile.X * CellSize + CellSize,	tile.Y + tile.seModY, tile.Z * CellSize + CellSize);
		}
	}

	_generatedMesh = std::make_unique<Mesh>(vertices, i, indices, j, uvs);
}

void GridMap::generateTexture()
{
	RenderTarget renderTarget(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec4(0.0f, 0.5f, 0.5f, 0.25f));
	renderTarget.beginDraw(_mapGenerationShader.get());

	//TODO: draw map texture
	//TODO: apply an uv map

	renderTarget.endDraw();
}
