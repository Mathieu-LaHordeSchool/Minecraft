#include <BlockDestroyer.h>

#include <iostream>
#include <GLFW/glfw3.h>

#include <Camera.h>
#include <Input/InputAction.h>
#include <Input/Inputs.h>
#include <Physics/RayHitResult.h>
#include <Scene/Cube.h>
#include <KeyCode.h>

BlockDestroyer::BlockDestroyer(Camera* cam, float range)
	: camera(cam)
	, range(range)
{
}

void BlockDestroyer::BindInput(Inputs* input)
{
	input->CreateInputAction("LMB", KeyCode::MOUSE_BUTTON_1, Mouse);

	input->GetInputbyName("LMB")->BindPressDownAction([this](auto t) {
		std::vector<RayHitResult> hits;
		bool collide = GetScene()->RaycastingCollision(
			camera->position,
			camera->position + (camera->direction * range),
			hits,
			1
		);

		if (!collide)
			return;

		RayHitResult hit = GetScene()->FindNeartest(camera->position, hits);
		hits.clear();

		if (!hit.cube->destructable)
			return;

		GetScene()->DestroyCube(hit.cube);
		GetScene()->DestroyCollision(hit.collision);
	});
}
