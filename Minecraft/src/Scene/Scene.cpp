#include "Scene/Scene.h"

#include <Mesh.h>
#include <Map.h>
#include <HandleObject.h>
#include <Physics/Collision.h>
#include <Physics/RayHitResult.h>
#include <Physics/BoxCollision.h>
#include <WorldChunck.h>
#include <Scene/Cube.h>
#include <MinecraftData.h>

#include <GLFW/glfw3.h>

void Scene::HandlesTick()
{
	time = (float)glfwGetTime();
	deltaTime = time - lastTime;

	if (deltaTime < 0.1f) {
		for (auto& h : SceneHandleObject)
			h->Tick({ time, deltaTime });
	}

	lastTime = time;
}
void Scene::HandlesBegin()
{
	for (auto& h : SceneHandleObject)
	{
		h->Begin();
	}
}
void Scene::HandlesBindInput(Inputs* input)
{
	for (auto& h : SceneHandleObject)
	{
		h->BindInput(input);
	}
}

void Scene::DestroyCube(Cube* cube)
{
	destroyedCube.push_back(cube);
}
void Scene::DestroyCollision(Collision* col)
{
	destroyedCollision.push_back(col);
}
void Scene::DestroyWorldChunk(WorldChunck* chk)
{
	destroyWorldWorld.push_back(chk);
}
void Scene::DestroyVectorObject()
{
	if (!destroyedCube.empty()) {
		for (auto& cube : destroyedCube)
			cube->chk->RemoveElement(cube->layer, cube);

		destroyedCube.clear();
	}

	if (!destroyedCollision.empty()) {
		for (auto& col : destroyedCollision)
			col->chk->RemoveCollision(col->layer, col);;

		destroyedCollision.clear();
	}

	if (!destroyWorldWorld.empty()) {
		for (auto& chk : destroyWorldWorld)
			ChunckMaps.remove(chk);
		
		destroyWorldWorld.clear();
	}
}

WorldChunck* Scene::CreateChunck()
{
	WorldChunck* newChunck = new WorldChunck(this);

	ChunckMaps.push_back(newChunck);
	return newChunck;
}

WorldChunck* Scene::GetWorldChunckPosition(glm::vec3 p)
{
	for (auto& chk : ChunckMaps)
	{
		glm::vec2 cPosition = chk->GetChunckPosition();

		glm::vec3 point1 = {
			(cPosition.x * 16.f) - 1,
			-999.f,
			(cPosition.y * 16.f) - 1
		};
		glm::vec3 point2 = {
			cPosition.x + 16.f + point1.x + 1,
			999.f,
			cPosition.y + 16.f + point1.z + 1
		};

		bool collide = BoxCollision::InCollision(p, point1, point2);

		if (collide)
			return chk;
	}

	return nullptr;
}

bool Scene::RaycastingCollision(glm::vec3 startPosition, glm::vec3 endPosition, std::vector<RayHitResult>& hits, int layer)
{
	std::vector<WorldChunck*> chuncks;
	RaycastingChuncks(startPosition, endPosition, chuncks);

	if (chuncks.size() < 1) {
		return false;
	}

	for (auto& chk : chuncks)
	{
		std::list chunckCollision = chk->GetCollisions()[layer];
		for (auto& cols : chunckCollision)
		{
			glm::vec3 intersectionPoint;
			if (cols->InIntersection(startPosition, endPosition, intersectionPoint)) {

				RayHitResult hit;
				hit.hitPosition = intersectionPoint;
				hit.position = cols->position;
				hit.normal = glm::normalize(hit.hitPosition - cols->position);
				hit.distance = glm::distance(startPosition, hit.position);
				hit.cube = cols->cube;
				hit.collision = cols;
				hit.chk = cols->chk;
				hits.push_back(hit);
			}
		}
	}

	return hits.size() > 0;
}
bool Scene::RaycastingCollision(glm::vec3 startPosition, glm::vec3 direction, float distance, std::vector<class RayHitResult>& hits, int layer)
{
	return RaycastingCollision(startPosition, startPosition + (direction * distance), hits, layer);
}
bool Scene::RaycastingChuncks(glm::vec3 startPosition, glm::vec3 endPosition, std::vector<WorldChunck*>& chuncks)
{
	for (auto& chk : ChunckMaps)
	{
		glm::vec2 cPosition = chk->GetChunckPosition();

		glm::vec3 point1 = {
			(cPosition.x * MC_CHUNk_BLEND_SIZE) - 1,
			-999.f,
			(cPosition.y * MC_CHUNk_BLEND_SIZE) - 1
		};
		glm::vec3 point2 = {
			cPosition.x + MC_CHUNk_BLEND_SIZE + point1.x + 1,
			999.f,
			cPosition.y + MC_CHUNk_BLEND_SIZE + point1.z + 1
		};

		glm::vec3 nan;
		bool collide = BoxCollision::InInteraction(startPosition, endPosition, point1, point2, nan);

		if (collide)
			chuncks.push_back(chk);
	}

	return chuncks.size() > 0;
}

Cube* Scene::CreateObject(glm::vec3 pos, int layer, WorldChunck* chk)
{
	Cube* newObject = new Cube(pos, layer, chk);

	if (chk->GetInstancedMap()[layer] == nullptr) {
		Map* newMap = new Map();
		chk->SetInstanceMap(layer, newMap);
		newMapInstancedCreated(layer, newMap);
	}

	chk->GetInstancedMap()[layer]->AddElement(newObject);
	return newObject;
}

RayHitResult Scene::FindNeartest(glm::vec3 position, std::vector<RayHitResult> objects)
{
	float distance = -1.f;
	RayHitResult result;

	for (auto& obj : objects)
	{
		float newDistance = glm::distance(position, obj.position);

		if (distance == -1.f) {

			distance = newDistance;
			result = obj;
		}
		else {

			if (newDistance < distance) {

				distance = newDistance;
				result = obj;
			}
		}
	}

	return result;
}

HandleObject* Scene::CreateHandleObjet(HandleObject* handle)
{
	SceneHandleObject.push_back(handle);
	handle->SetScene(this);

	return handle;
}
Collision* Scene::CreateCollionObject(WorldChunck* chk, Collision* collision)
{
	collision->chk = chk;
	chk->SetCollision(collision->layer, collision);
	return collision;
}
