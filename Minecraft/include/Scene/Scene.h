#pragma once

#include <vector>
#include <list>

#include <unordered_map>
#include <functional>
#include <tuple>

#include <glm/glm.hpp>

#include <HandleTimer.h>

class Cube;
class Mesh;
class Collision;

class Scene
{
public:
	explicit Scene()  = default;
	~Scene() noexcept = default;

public:
	void HandlesTick();
	void HandlesBegin();
	void HandlesBindInput(class Inputs* input);

	void DestroyCube(Cube* cube);
	void DestroyCollision(Collision* col);
	void DestroyWorldChunk(class WorldChunck* chk);
	void DestroyVectorObject();

	class WorldChunck* CreateChunck();
	class WorldChunck* GetWorldChunckPosition(glm::vec3 p);

	bool RaycastingCollision(glm::vec3 startPosition, glm::vec3 endPosition, std::vector<class RayHitResult>& hits, int layer);
	bool RaycastingCollision(glm::vec3 startPosition, glm::vec3 direction, float distance, std::vector<class RayHitResult>& hits, int layer);
	
	bool RaycastingChuncks(glm::vec3 startPosition, glm::vec3 endPosition, std::vector<class WorldChunck*>& chuncks);

	Cube* CreateObject(glm::vec3 pos, int id, class WorldChunck* chk);

	RayHitResult FindNeartest(glm::vec3 position, std::vector<RayHitResult> objects);

	template<typename T, typename ...ARG> T* CreateHandleObject(ARG... arguments) {
		return dynamic_cast<T*>(CreateHandleObjet(new T(arguments...)));
	}
	template<typename T> T* CreateCollisionObject(Cube* cube, int collisionLayer, class WorldChunck* chk) {
		return dynamic_cast<T*>(CreateCollionObject(chk, new T(cube, collisionLayer)));
	}

public:
	inline std::vector<class HandleObject*>		GetAllHandleObject()	const { return SceneHandleObject;	}
	inline std::list<class WorldChunck*>		GetChuncks()			const { return ChunckMaps;			}

	inline HandleTimer							GetHandleTimer()		const { return { time, deltaTime }; }
	inline float								GetTime()				const { return time;				}
	inline float								GetDeltaTime()			const { return deltaTime;			}

public:
	std::function<void(int index, class Map* map)> newMapInstancedCreated;

private:
	std::list<class WorldChunck*>	 ChunckMaps;
	std::vector<class HandleObject*> SceneHandleObject;

	std::vector<std::tuple<class WorldChunck*, int, Cube*>> addCubeList;

	std::vector<Cube*> destroyedCube;
	std::vector<Collision*> destroyedCollision;
	std::vector<class WorldChunck*> destroyWorldWorld;

	float time = 0.f;
	float lastTime = 0.f;
	float deltaTime = 0.f;

private:
	class HandleObject* CreateHandleObjet(HandleObject* handle);
	class Collision* CreateCollionObject(class WorldChunck* chk, Collision* collision);
};