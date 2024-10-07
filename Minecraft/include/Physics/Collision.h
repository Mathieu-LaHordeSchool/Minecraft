#pragma once

#include <glm/glm.hpp>
#include <Scene/Cube.h>

class Collision
{
public:
	explicit Collision(Cube* cube, int layer)
		: layer(layer)
	{
		if (cube) {
			cube->collision = this;
			position = cube->position;
			this->cube = cube;
		}
	}
	~Collision() noexcept = default;

public:
	virtual bool InCollision(glm::vec3 p) = 0;
	virtual bool InIntersection(glm::vec3 p1, glm::vec3 p2, glm::vec3& intersection) = 0;

public:
	glm::vec3 position;
	int layer;
	Cube* cube;
	class WorldChunck* chk;
};
