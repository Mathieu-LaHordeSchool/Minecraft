#pragma once

#include <Physics/Collision.h>

class BoxCollision : public Collision
{
public:
	explicit BoxCollision(class Cube* cube, int layer)
		: Collision(cube, layer)
	{}
	BoxCollision() noexcept = default;

public:
	glm::vec3 p1 = glm::vec3(1.f);
	glm::vec3 p2 = glm::vec3(-1.f);

public:
	bool InCollision(glm::vec3 p) override;
	bool InIntersection(glm::vec3 start, glm::vec3 end, glm::vec3& intersection) override;

	static bool InInteraction(glm::vec3 start, glm::vec3 end, glm::vec3 point1, glm::vec3 point2, glm::vec3& intersection);
	static bool InCollision(glm::vec3 point, glm::vec3 point1, glm::vec3 point2);
};
