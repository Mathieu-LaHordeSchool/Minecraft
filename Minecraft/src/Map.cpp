#include "Map.h"

#include <Buffer.h>
#include <vector>
#include <algorithm>

#include <Scene/Cube.h>

Map::Map()
{
	mapBuffer = new Buffer();
	mapBuffer->InitBuffer(sizeof(glm::vec3) * 2700, nullptr);

	for (int i = 0; i < 2700; i++) {
		map.push_back(glm::vec3(-9999999));
	}
}

void Map::AddElement(Cube* cube)
{
	mapBuffer->SetMapBuffer(numInstance, cube->position);
	map[numInstance] = cube->position;
	numInstance++;
}
void Map::RemoveElement(Cube* cube)
{
	auto it = std::find(map.begin(), map.end(), cube->position);

	if (it != map.end()) {
		int index = std::distance(map.begin(), it);
		mapBuffer->SetMapBuffer(index, map[numInstance - 1]);
		map[index] = map[numInstance - 1];

		numInstance--;
	}
}