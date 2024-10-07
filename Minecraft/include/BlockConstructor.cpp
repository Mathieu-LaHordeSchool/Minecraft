#include "BlockConstructor.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <Input/Inputs.h>
#include <Camera.h>
#include <Physics/RayHitResult.h>
#include <Physics/BoxCollision.h>
#include <WorldChunck.h>
#include <MinecraftData.h>
#include <KeyCode.h>

BlockConstructor::BlockConstructor(Camera* cam, float range)
	: cam(cam)
	, range(range)
{ }

void BlockConstructor::BindInput(Inputs* input)
{
	input->CreateInputAction("RMB", KeyCode::MOUSE_BUTTON_2, Mouse);

	input->CreateInputAction("dirt",	KeyCode::NUM_1);
	input->CreateInputAction("grass",	KeyCode::NUM_2);
	input->CreateInputAction("log",		KeyCode::NUM_3);
	input->CreateInputAction("leaf",	KeyCode::NUM_4);

	input->GetInputbyName("RMB")->BindPressDownAction([this](auto t) {
		PlaceBlock();
	});

	input->GetInputbyName("dirt")->BindPressDownAction([this](auto t)	{ blockIndex = MC_DIRT_LAYER;	});
	input->GetInputbyName("grass")->BindPressDownAction([this](auto t)	{ blockIndex = MC_GRASS_LAYER;	});
	input->GetInputbyName("log")->BindPressDownAction([this](auto t)	{ blockIndex = MC_LOG_LAYER;	});
	input->GetInputbyName("leaf")->BindPressDownAction([this](auto t)	{ blockIndex = MC_LEAF_LAYER;	});
}

void BlockConstructor::PlaceBlock()
{
	std::vector<RayHitResult> hits;
	bool collide = GetScene()->RaycastingCollision(
		cam->position,
		cam->position + (cam->direction * range),
		hits,
		1
	);

	if (!collide)
		return;

	RayHitResult hit = GetScene()->FindNeartest(cam->position, hits);
	hits.clear();

	glm::vec3 posNewBlock = hit.position;
	glm::vec3 normal = hit.normal;
	glm::vec3 absNormal = abs(normal);

	if (absNormal.x > absNormal.y && absNormal.x > absNormal.z) {
		posNewBlock.x += normal.x > 0.f ? 2.f : -2.f;
	}
	else if (absNormal.y > absNormal.x && absNormal.y > absNormal.z) {
		posNewBlock.y += normal.y > 0.f ? 2.f : -2.f;
	}
	else if (absNormal.z > absNormal.x && absNormal.z > absNormal.y) {
		posNewBlock.z += normal.z > 0.f ? 2.f : -2.f;
	}

	if (abs(posNewBlock.y) >= 100.f)
		return;

	glm::vec3 blockDistance = posNewBlock - cam->position;

	if (glm::distance(cam->position, posNewBlock) <= 1.5f)
		return;

	float downDistance = glm::distance(cam->position - glm::vec3(0.f, 2.f, 0.f), posNewBlock);
	if (downDistance <= 1.5f)
		return;

	WorldChunck* chunck = GetScene()->GetWorldChunckPosition(posNewBlock);
	Cube* cube = GetScene()->CreateObject(posNewBlock, blockIndex, chunck);
	GetScene()->CreateCollisionObject<BoxCollision>(cube, 1, chunck);
}
