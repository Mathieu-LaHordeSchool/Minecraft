
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

#include <glm/glm.hpp>
#include <list>

#include <Camera.h>
#include <BlockConstructor.h>
#include <BlockDestroyer.h>

#include <MinecraftWindow.h>
#include <Render.h>
#include <Mesh.h>
#include <Scene/Scene.h>
#include <Texture.h>
#include <Input/Inputs.h>
#include <PerlinNoise.h>
#include <WorldChunck.h>
#include <MinecraftData.h>

int main(int argc, char** argv)
{
	Inputs*				inputs		= new Inputs();
	MinecraftWindow*	window		= new MinecraftWindow("Minecraft", 1920 / 2, 1080 / 2);
	Renderer*			Render3D	= new Renderer();

	Texture* grassTexture	= new Texture("Textures/Albedos/grassPatron.png");
	Texture* waterTexture	= new Texture("Textures/Albedos/water.png");
	Texture* logTexture		= new Texture("Textures/Albedos/Log.png");
	Texture* leafTexture	= new Texture("Textures/Albedos/leaf.png");
	Texture* stoneTexture	= new Texture("Textures/Albedos/Stone.png");
	Texture* bedrockTexture = new Texture("Textures/Albedos/Bedrock.png");

	Mesh meshWater		= Mesh("Models/default.obj",	waterTexture);
	Mesh meshWaterUp	= Mesh("Models/waterUp.obj",	waterTexture);
	Mesh meshDirtCube	= Mesh("Models/Dirt.obj",		grassTexture);
	Mesh meshGrassCube	= Mesh("Models/Grass.obj",		grassTexture);
	Mesh meshLog		= Mesh("Models/log.obj",		logTexture);
	Mesh meshLeaf		= Mesh("Models/default.obj",	leafTexture);
	Mesh meshStone		= Mesh("Models/default.obj",	stoneTexture);
	Mesh meshBedrock	= Mesh("Models/default.obj",	bedrockTexture);

	Render3D->BindMeshIndex(&meshDirtCube,	MC_DIRT_LAYER);
	Render3D->BindMeshIndex(&meshGrassCube,	MC_GRASS_LAYER);
	Render3D->BindMeshIndex(&meshWater,		MC_WATER_LAYER);
	Render3D->BindMeshIndex(&meshLog,		MC_LOG_LAYER);
	Render3D->BindMeshIndex(&meshWaterUp,	MC_WATER_UP_LAYER);
	Render3D->BindMeshIndex(&meshLeaf,		MC_LEAF_LAYER);
	Render3D->BindMeshIndex(&meshStone,		MC_STONE_LAYER);
	Render3D->BindMeshIndex(&meshBedrock,	MC_BEDROCK_LAYER);

	std::list<glm::ivec2> worldChunlPosition;

	Scene* mainScene = new Scene();

	Camera*				camera		= mainScene->CreateHandleObject<Camera>(1920 / 2, 1080 / 2, 0.1f, 100.f, 70.f, window);
	BlockConstructor*	constructor	= mainScene->CreateHandleObject<BlockConstructor>(camera, 12.f);
	BlockDestroyer*		destroyer	= mainScene->CreateHandleObject<BlockDestroyer>(camera, 12.f);

	camera->position = {
		0.f,
		200.f,
		0.f
	};

	PerlinNoise perlin(400);

	for (int chunkX = 0; chunkX <= MC_CHUNk_SIZE; chunkX++)
	{
		for (int chunkZ = 0; chunkZ <= MC_CHUNk_SIZE; chunkZ++)
		{
			// one chunck
			WorldChunck* newChunck = mainScene->CreateChunck();
			
			newChunck->InitRenderObjects(Render3D->GetMeshMap(), Render3D->GetRenderDataObject());
			newChunck->SpawnChunckBlock(perlin, MC_WATER_HEIGHT, glm::vec2(chunkX, chunkZ), glm::vec2(MC_CHUNk_SIZE));
			
			// worldChunk.push_back(newChunck);
			worldChunlPosition.push_back({ chunkX, chunkZ });
		}
	}

	mainScene->HandlesBegin();
	mainScene->HandlesBindInput(inputs);

	do
	{
		window->PoolEvent();
		mainScene->HandlesTick();
		inputs->UpdateInputs(mainScene->GetHandleTimer(), window);
		mainScene->DestroyVectorObject();

		Render3D->RenderScene(mainScene, camera);
		window->SwapBuffer();

	} while (!window->IsClose());

	window->Terminate();

	return 0;
}

#ifdef NDEBUG
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	return main(0, nullptr);
}
#endif /* NDEBUG */