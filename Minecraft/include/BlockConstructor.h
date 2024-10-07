#pragma once

#include <HandleObject.h>

class Camera;

class BlockConstructor : public HandleObject
{
public:
	explicit BlockConstructor(Camera* cam, float range);
	~BlockConstructor() noexcept = default;

public:
	void BindInput(Inputs* input) override;
	void PlaceBlock();

private:
	Camera* cam;
	float range;
	int blockIndex = 1;
};