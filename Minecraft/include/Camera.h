#pragma once

#include <glm/glm.hpp>
#include <functional>

#include <HandleObject.h>

class Camera : public HandleObject
{
public:
	explicit Camera(float w, float h, float zNear, float zFar, float fov, class MinecraftWindow* win);
	~Camera() noexcept = default;

	void Begin		()							override;
	void Tick		(class HandleTimer time)	override;
	void BindInput	(class Inputs* input)		override;

	bool CollideBlock(glm::vec3 direction);
	bool IsGrounded(glm::vec3& pos);
	bool IsCelling();

public:
	float GetRatio() const { return Width / Height; }

public:
	float Width, Height;
	float zNear, zFar;
	float Fov;

	float moveSpeed   = 10.f;
	float rotateSpeed = 8.f;

	glm::vec3 fwdVector;
	glm::vec3 upVector;
	glm::vec3 rigVector;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 direction;
	glm::vec3 velocity;

	glm::ivec2 chunckPosition;

	class MinecraftWindow* window;

	std::function<void(const glm::vec2&)> chunckPositionChanged;

private:
	bool cursorEnable = false;
	bool canRotate = true;
	bool grounded = false;
};