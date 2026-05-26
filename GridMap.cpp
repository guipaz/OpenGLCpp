#include "GridMap.h"
#include <random>
#include <iostream>
#include "RenderTarget.h"
#include "Constants.h"
#include "Utils.h"

using namespace glm;

GridMap::GridMap(unsigned int columns, unsigned int rows, float cellSize, float heightStep)
{
	this->columns = columns;
	this->rows = rows;
	this->cellSize = cellSize;
	this->heightStep = heightStep;

	generateCells();
	generateMesh();

	mesh = _generatedMesh.get();
}

void GridMap::generateCells()
{
	//std::random_device rd;
	std::mt19937 gen(1);

	_cells = std::vector<GridMapCell>(columns * rows);
	for (size_t z = 0; z < rows; ++z)
	{
		for (size_t x = 0; x < columns; ++x)
		{
			// generate a random y
			std::uniform_int_distribution distr(0, 2);
			float random_y = distr(gen) * heightStep;

			float x1 = 0.5f * (x % 2);
			float z1 = 0.5f * (z % 2);
			float x2 = 0.5f * (x % 2) + 0.5f;
			float z2 = 0.5f * (z % 2) + 0.5f;

			_cells[z * columns + x] = GridMapCell {
				(unsigned int)x, random_y, (unsigned int)z, // position
				0,											// texture
				vec2(x1, z1),
				vec2(x2, z1),
				vec2(x1, z2),
				vec2(x2, z2),
				0, 0, 0, 0 };								// height mods
		}
	}

	// DEBUGGING
	_cells[2 * columns + 2].smoothEdge = false;
	_cells[3 * columns + 2].smoothEdge = false;
	_cells[2 * columns + 3].smoothEdge = false;
	_cells[3 * columns + 3].smoothEdge = false;

	_cells[2 * columns + 2].y = .5f;
	_cells[3 * columns + 2].y = .5f;
	_cells[2 * columns + 3].y = .5f;
	_cells[3 * columns + 3].y = .5f;

	// edge smoothing
	int iWidth = static_cast<int>(columns);
	int iHeight = static_cast<int>(rows);
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
				median_y += _cells[iNw].y;
			}
			if (applyNe)
			{
				++iMedian;
				median_y += _cells[iNe].y;
			}
			if (applySw)
			{
				++iMedian;
				median_y += _cells[iSw].y;
			}
			if (applySe)
			{
				++iMedian;
				median_y += _cells[iSe].y;
			}
			median_y /= iMedian;

			if (applyNw) _cells[iNw].seModY = median_y - _cells[iNw].y;
			if (applyNe) _cells[iNe].swModY = median_y - _cells[iNe].y;
			if (applySw) _cells[iSw].neModY = median_y - _cells[iSw].y;
			if (applySe) _cells[iSe].nwModY = median_y - _cells[iSe].y;
		}
	}
}

void GridMap::generateMesh()
{
	//TODO: calculate option for single-vertex edged tiles
	// vertices, indices, uvs
	const size_t vertexCount = columns * rows * 4;
	const size_t indexCount = columns * rows * 6;

	vec3* vertices = new glm::vec3[columns * rows * 4];
	unsigned int* indices = new unsigned int[columns * rows * 2 * 3]; // * 2 triangles * 3 vertices per triangle
	vec2* uvs = new glm::vec2[columns * rows * 4];

	unsigned int i = 0;
	unsigned int j = 0;
	
	float uvStepX = 1.0f / columns;
	float uvStepY = 1.0f / rows;

	for (size_t y = 0; y < rows; ++y)
	{
		for (size_t x = 0; x < columns; ++x)
		{
			GridMapCell cell = _cells[y * columns + x];

			// set the 6 indices for this tile's 2 triangles
			indices[j++] = i;
			indices[j++] = i + 1;
			indices[j++] = i + 2;
			indices[j++] = i + 1;
			indices[j++] = i + 2;
			indices[j++] = i + 3;

			uvs[i]		= cell.textureUVA;
			uvs[i + 1] = cell.textureUVB;
			uvs[i + 2] = cell.textureUVC;
			uvs[i + 3] = cell.textureUVD;

			//uvs[i]		= vec2(cell.x			* uvStepX, cell.z		* uvStepY);
			//uvs[i + 1]	= vec2((cell.x + 1)		* uvStepX, cell.z		* uvStepY);
			//uvs[i + 2]	= vec2(cell.x			* uvStepX, (cell.z + 1)	* uvStepY);
			//uvs[i + 3]	= vec2((cell.x + 1)		* uvStepX, (cell.z + 1)	* uvStepY);

			// set the 4 vertices for this tile
			vertices[i++] = vec3(cell.x * cellSize,				cell.y + cell.nwModY, cell.z * cellSize);
			vertices[i++] = vec3(cell.x * cellSize + cellSize,	cell.y + cell.neModY, cell.z * cellSize);
			vertices[i++] = vec3(cell.x * cellSize,				cell.y + cell.swModY, cell.z * cellSize + cellSize);
			vertices[i++] = vec3(cell.x * cellSize + cellSize,	cell.y + cell.seModY, cell.z * cellSize + cellSize);
		}
	}

	_generatedMesh = std::make_unique<Mesh>(vertices, i, indices, j, uvs);
}