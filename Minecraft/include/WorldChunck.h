#pragma once

#include <glm/glm.hpp>

#include <functional>
#include <unordered_map>
#include <list>

class WorldChunck
{
public:
	explicit WorldChunck(class Scene* scn);
	~WorldChunck() noexcept = default;

	void SpawnChunckBlock(class PerlinNoise, float waterHeight, glm::vec2 chunckPos, glm::vec2 chunckSize);

	void InitRenderObjects(std::unordered_map<int, class Mesh*> meshMap, class RenderObjectData* renderData);
	void InitRenderObject(class Mesh* mesh, int indexVAO, class RenderObjectData* renderData);

	void InitInstancedMap(int indexVAO, class Map* map);

	void SetInstanceMap(int key, class Map* value);
	void SetCollision(int key, class Collision* cols);
	void RemoveCollision(int key, class Collision* col);
	void RemoveElement(int key, class Cube* cube);

public:
	inline std::unordered_map<int, class Map*>					GetInstancedMap()		const { return InstancedMaps;		}
	inline std::unordered_map<int, std::list<class Collision*>> GetCollisions()			const { return CollisionObjects;	}
	inline std::unordered_map<int, class VertexArrayObject*>	GetVertexArrayObjects()	const { return VAOs;				}
	inline glm::vec2											GetChunckPosition()		const { return chunkPosition;		}	

private:
	glm::vec2 chunkPosition;

	class Scene* mainScene;

	std::unordered_map<int, class Map*>						InstancedMaps;
	std::unordered_map<int, std::list<class Collision*>>	CollisionObjects;
	std::unordered_map<int, class VertexArrayObject*>		VAOs;
};
