#pragma once

#include <HandleTimer.h>
#include <Input/Inputs.h>
#include <Scene/Scene.h>

class HandleObject
{
public:
	virtual void Begin()					{ };
	virtual void Tick(HandleTimer time)		{ };
	virtual void BindInput(Inputs* input)	{ };

public:
	inline Scene* GetScene() const			{ return owerScene; }
	inline void SetScene(Scene* scn)	{ owerScene = scn;	}

private:
	Scene* owerScene;
};