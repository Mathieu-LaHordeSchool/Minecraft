#include "Camera.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <MinecraftWindow.h>
#include <Input/Inputs.h>
#include <Input/InputAction.h>
#include <HandleTimer.h>
#include <MinecraftWindow.h>
#include <Physics/RayHitResult.h>
#include <Scene/Cube.h>
#include <KeyCode.h>
#include <MinecraftData.h>

#define PI 3.14159265

Camera::Camera(float w, float h, float zNear, float zFar, float fov, MinecraftWindow* win)
	: Width(w), Height(h)
	, zNear(zNear), zFar(zFar)
	, Fov(fov)
	, window(win)
{}

void Camera::Begin()
{
	//if (!cursorEnable)
	//	glfwSetInputMode(window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//else
	//	glfwSetInputMode(window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	std::vector<RayHitResult> hits;
	if (GetScene()->RaycastingCollision(position, glm::vec3(0.f, -1.f, 0.f), 100000.f, hits, 1)) {
		auto current = GetScene()->FindNeartest(position, hits);
		position.y = current.position.y + 4.f;
	}
}
void Camera::Tick(HandleTimer time)
{
	glm::ivec2 chkPos(
		(int)(position.x / (MC_CHUNk_SIZE * 2.f)),
		(int)(position.z / (MC_CHUNk_SIZE * 2.f))
	);

	if (chunckPosition != chkPos) {
		if (chunckPositionChanged)
			chunckPositionChanged(chkPos);

		chunckPosition = chkPos;
	}

	fwdVector = glm::normalize(position - (position + glm::vec3(direction.x, 0.f, direction.z)));
	upVector  = glm::vec3(0.f, 1.f, 0.f);
	rigVector = glm::cross(fwdVector, upVector);

	std::cout << time.deltaTime << std::endl;
	position += velocity * time.deltaTime;
	glm::vec3 pos;
	grounded = IsGrounded(pos);

	if (grounded) {
		position.y = pos.y + 4.f;
		velocity.y = 0.f;
	}
	else {
		velocity.y = glm::clamp(velocity.y - 25.f * time.deltaTime, -100.f, 100.f);
	}
}
void Camera::BindInput(Inputs* input)
{
	input->CreateInputAction("z", KeyCode::W);
	input->CreateInputAction("q", KeyCode::A);
	input->CreateInputAction("s", KeyCode::S);
	input->CreateInputAction("d", KeyCode::D);
	input->CreateInputAction("space", KeyCode::SPACE);
	input->CreateInputAction("echap", KeyCode::ESCAPE);

	input->GetInputbyName("z")->BindPressAction([this](HandleTimer t) {

		if(!CollideBlock(-fwdVector))
			position -= fwdVector * t.deltaTime * moveSpeed;
	});
	input->GetInputbyName("q")->BindPressAction([this](HandleTimer t) {

		if (!CollideBlock(rigVector))
			position += rigVector * t.deltaTime * moveSpeed; 
	});
	input->GetInputbyName("s")->BindPressAction([this](HandleTimer t) {

		if (!CollideBlock(fwdVector))
			position += fwdVector * t.deltaTime * moveSpeed; 
	});
	input->GetInputbyName("d")->BindPressAction([this](HandleTimer t) {

		if (!CollideBlock(-rigVector))
			position -= rigVector * t.deltaTime * moveSpeed;
	});

	input->GetInputbyName("space")->BindPressAction([this](HandleTimer t) { 
		if (!IsCelling() && grounded)
			velocity.y += 10.f;
	});

	input->BindMouseDeltaPosition([this](float dx, float dy, HandleTimer t) {
		if (!canRotate)
			return;

		rotation.x = glm::clamp(rotation.x + rotateSpeed * t.deltaTime * -dy, -89.f, 89.f);
		rotation.y += rotateSpeed * t.deltaTime * dx;

		direction = glm::vec3(
			cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y)),
			sin(glm::radians(rotation.x)),
			cos(glm::radians(rotation.x)) * sin(glm::radians(rotation.y))
		);
	});

	input->GetInputbyName("echap")->BindPressDownAction([this](HandleTimer t) {
		cursorEnable = !cursorEnable;
		canRotate = !canRotate;

		if (!cursorEnable)
			glfwSetInputMode(window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	});
}

bool Camera::CollideBlock(glm::vec3 direction)
{
	glm::vec3 crossVec = glm::abs(glm::cross(direction, glm::vec3(0.f, 1.f, 0.f)) * -1.f) * 0.5f;
	std::vector<RayHitResult> hits;
	glm::vec3 downPostion = {
		position.x,
		position.y - 2.f,
		position.z
	};
	
	bool collideD1 = GetScene()->RaycastingCollision(downPostion + crossVec, (downPostion + crossVec) + (direction * 1.2f), hits, 1);
	bool collideD2 = GetScene()->RaycastingCollision(downPostion - crossVec, (downPostion - crossVec) + (direction * 1.2f), hits, 1);
	bool collideU1 = GetScene()->RaycastingCollision(position + crossVec, (position + crossVec) + (direction * 1.2f), hits, 1);
	bool collideU2 = GetScene()->RaycastingCollision(position - crossVec, (position - crossVec) + (direction * 1.2f), hits, 1);

	return collideD1 || collideD2 || collideU1 || collideU2;
}
bool Camera::IsGrounded(glm::vec3& pos)
{
	std::vector<RayHitResult> hits;

	glm::vec3 fPos = position - fwdVector * 0.5f;
	glm::vec3 bPos = position + fwdVector * 0.5f;
	glm::vec3 rPos = position + rigVector * 0.5f;
	glm::vec3 lPos = position - rigVector * 0.5f;

	bool collidef = GetScene()->RaycastingCollision(fPos, fPos + (-upVector * 3.f), hits, 1);
	bool collideb = GetScene()->RaycastingCollision(bPos, bPos + (-upVector * 3.f), hits, 1);
	bool collider = GetScene()->RaycastingCollision(rPos, rPos + (-upVector * 3.f), hits, 1);
	bool collidel = GetScene()->RaycastingCollision(lPos, lPos + (-upVector * 3.f), hits, 1);

	if (hits.size() > 0)
		pos.y = hits[0].cube->position.y;

	return collidef || collideb || collider || collidel;
}
bool Camera::IsCelling()
{
	std::vector<RayHitResult> hits;

	glm::vec3 fPos = position - fwdVector * 0.5f;
	glm::vec3 bPos = position + fwdVector * 0.5f;
	glm::vec3 rPos = position + rigVector * 0.5f;
	glm::vec3 lPos = position - rigVector * 0.5f;

	bool collidef = GetScene()->RaycastingCollision(fPos, fPos + (upVector * 2.f), hits, 1);
	bool collideb = GetScene()->RaycastingCollision(bPos, bPos + (upVector * 2.f), hits, 1);
	bool collider = GetScene()->RaycastingCollision(rPos, rPos + (upVector * 2.f), hits, 1);
	bool collidel = GetScene()->RaycastingCollision(lPos, lPos + (upVector * 2.f), hits, 1);

	return collidef || collideb || collider || collidel;
}
