#pragma once

#include <glm/glm.hpp>

class Cube
{
public:
	explicit Cube(glm::vec3 position, int layer, class WorldChunck* chk);
	~Cube() noexcept = default;

public:
	glm::vec3 position;
	class Collision* collision;
	class WorldChunck* chk;
	bool destructable = true;

	int layer;
	int numInstance;
};