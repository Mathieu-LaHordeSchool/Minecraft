#pragma once

#include <HandleObject.h>

class Camera;

class BlockDestroyer : public HandleObject
{
public:
	explicit BlockDestroyer(Camera* cam, float range);
	BlockDestroyer() noexcept = default;

public:
	void BindInput(Inputs* input);

private:
	Camera* camera;
	float range;
};
