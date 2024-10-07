#pragma once

#include <glm/glm.hpp>

class RayHitResult 
{
public:
	glm::vec3 hitPosition;
	glm::vec3 position;
	glm::vec3 normal;

	float distance;

	class Cube* cube;
	class Collision* collision;
	class WorldChunck* chk;
};
