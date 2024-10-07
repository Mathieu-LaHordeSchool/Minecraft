#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <list>
#include <VertexArray.h>
#include <unordered_map>
#include <RenderObjectData.h>

class ShaderProgram;
class MinecraftWindow;
class Scene;
class Camera;

class Renderer
{
public:
	explicit Renderer();
	~Renderer() noexcept = default;

public:
	void RenderObjects(class Map* map, Mesh* mesh, VertexArrayObject* vao);
	void RenderScene(Scene* scene, Camera* cam);
	void BindMeshIndex(Mesh* mesh, int meshIndex);

public:
	inline std::unordered_map<int, Mesh*>	GetMeshMap()			const { return meshMap;		}
	inline RenderObjectData*				GetRenderDataObject()	const { return renderData;	}

private:
	ShaderProgram* shaderProgram;
	RenderObjectData* renderData;

	std::unordered_map<int, Mesh*> meshMap;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
};