#include <WorldChunck.h>

#include <Scene/Cube.h>
#include <Scene/Scene.h>
#include <Map.h>
#include <Physics/Collision.h>
#include <Physics/BoxCollision.h>
#include <RenderObjectData.h>
#include <MinecraftData.h>

#include <VertexArray.h>
#include <Buffer.h>

#include <PerlinNoise.h>

float randomFloat()
{
	return (float)(rand()) / (float)(RAND_MAX);
}

WorldChunck::WorldChunck(Scene* scn)
	: mainScene(scn)
{
}

void WorldChunck::SpawnChunckBlock(PerlinNoise perlin, float waterHeight, glm::vec2 chunckPos, glm::vec2 chunckSize)
{
	chunkPosition = chunckPos;

	mainScene->newMapInstancedCreated = [this](int index, Map* map) {
		InitInstancedMap(index, map);
	};

	for (size_t px = 0; px < chunckSize.x; px++)
	{
		for (size_t pz = 0; pz < chunckSize.y; pz++)
		{
			float x = (chunckPos.x * chunckSize.x) + px;
			float z = (chunckPos.y * chunckSize.y) + pz;

			int height = std::lerp(-4.f, 8.f, perlin.noise(x * 0.1f, 0.f, z * 0.1f)) * MC_BLEND_CUBE_SCALE;
			if (abs(height) % 2 == 1)
				height++;

			// dirt layer
			for (size_t dirtLayer = 0; dirtLayer < 4; dirtLayer++)
			{
				Cube* dirt = mainScene->CreateObject(glm::vec3(x * MC_BLEND_CUBE_SCALE, height - ((dirtLayer + 1) * MC_BLEND_CUBE_SCALE), z * MC_BLEND_CUBE_SCALE), MC_DIRT_LAYER, this);
				mainScene->CreateCollisionObject<BoxCollision>(dirt, 1, this);
			}

			// stone layer
			int stoneLayer = 0;
			Cube* stone;
			do
			{
				stone = mainScene->CreateObject(glm::vec3(x * MC_BLEND_CUBE_SCALE, (height) - ((stoneLayer + 5) * MC_BLEND_CUBE_SCALE), z * MC_BLEND_CUBE_SCALE), MC_STONE_LAYER, this);
				mainScene->CreateCollisionObject<BoxCollision>(stone, 1, this);
				stoneLayer++;

				if (!stone)
					break;

			} while (abs(stone->position.y) < 30.f);

			// bedrock layer
			Cube* bedRock = mainScene->CreateObject(glm::vec3(x * MC_BLEND_CUBE_SCALE, -32.f, z * MC_BLEND_CUBE_SCALE), MC_BEDROCK_LAYER, this);
			bedRock->destructable = false;
			mainScene->CreateCollisionObject<BoxCollision>(bedRock, 1, this);

			if (height > waterHeight) {
				// grass Layer
				Cube* grass = mainScene->CreateObject(glm::vec3(x * MC_BLEND_CUBE_SCALE, height, z * MC_BLEND_CUBE_SCALE), MC_GRASS_LAYER, this);
				mainScene->CreateCollisionObject<BoxCollision>(grass, 1, this);

				if (randomFloat() > .99f) {
					// Trees
					for (size_t logHeight = 0; logHeight < 7; logHeight++)
					{
						Cube* log = mainScene->CreateObject(glm::vec3(x * MC_BLEND_CUBE_SCALE, height + ((logHeight + 1) * MC_BLEND_CUBE_SCALE), z * MC_BLEND_CUBE_SCALE), MC_LOG_LAYER, this);
						mainScene->CreateCollisionObject<BoxCollision>(log, 1, this);
					}

					for (size_t lx = 0; lx < 3; lx++)
					{
						for (size_t ly = 0; ly < 6; ly++)
						{
							for (size_t lz = 0; lz < 3; lz++)
							{
								glm::vec3 leafPos = glm::vec3((x + lx - 1) * MC_BLEND_CUBE_SCALE, (height + ((ly - 1) * MC_BLEND_CUBE_SCALE)) + (7.f * MC_BLEND_CUBE_SCALE), (z + lz - 1) * MC_BLEND_CUBE_SCALE);

								Cube* leaf = mainScene->CreateObject(leafPos, MC_LEAF_LAYER, this);;
								mainScene->CreateCollisionObject<BoxCollision>(leaf, 1, this);
							}
						}
					}

				}
			}
			else {
				// dirt buttom water
				Cube* dirt = mainScene->CreateObject(glm::vec3(x * MC_BLEND_CUBE_SCALE, height, z * MC_BLEND_CUBE_SCALE), MC_DIRT_LAYER, this);
				mainScene->CreateCollisionObject<BoxCollision>(dirt, 1, this);

				// water layers
				do
				{
					height += MC_BLEND_CUBE_SCALE;
					if (height == waterHeight + MC_BLEND_CUBE_SCALE)
						mainScene->CreateObject(glm::vec3(x * MC_BLEND_CUBE_SCALE, height, z * MC_BLEND_CUBE_SCALE), MC_WATER_UP_LAYER, this);
					else
						mainScene->CreateObject(glm::vec3(x * MC_BLEND_CUBE_SCALE, height, z * MC_BLEND_CUBE_SCALE), MC_WATER_LAYER, this);

				} while (abs(height) >= abs(waterHeight));
			}
		}
	}
}

void WorldChunck::InitRenderObjects(std::unordered_map<int, Mesh*> meshMap, RenderObjectData* renderData)
{
	for (auto& [key, value] : meshMap)
	{
		InitRenderObject(value, key, renderData);
		
		Map* newMap = new Map();
		InitInstancedMap(key, newMap);
		SetInstanceMap(key, newMap);
	}
}
void WorldChunck::InitRenderObject(Mesh* mesh, int indexVAO, RenderObjectData* renderData)
{
	Buffer* verticeBuffer	= renderData->GetOrCreateVertexBuffer(mesh->Vertices);
	Buffer* colorBuffer		= renderData->GetOrCreateColorBuffer(mesh->Colors);
	Buffer* TextsUv			= renderData->GetOrCreateTextureBuffer(mesh->TexsUv);
	Buffer* ElementsBuffer	= renderData->GetOrCreateElementBuffer(mesh->Elements);
	// Buffer* normalBuffer	= renderData->GetOrCreateNormalBuffer(mesh->Normals);


	VertexArrayObject* newVAO = new VertexArrayObject();
	newVAO->BindElementBuffer(ElementsBuffer);

	newVAO->BindingBuffer<float>(0, 0, verticeBuffer, 3);
	newVAO->BindingBuffer<float>(1, 0, colorBuffer, 4);
	newVAO->BindingBuffer<float>(2, 0, TextsUv, 2);
	// newVAO->BindingBuffer<float>(3, 0, normalBuffer, 3);

	newVAO->AttributeBinding(0, 0, 3, GL_FLOAT, GL_FALSE, 0);
	newVAO->AttributeBinding(1, 1, 4, GL_FLOAT, GL_FALSE, 0);
	newVAO->AttributeBinding(2, 2, 2, GL_FLOAT, GL_FALSE, 0);
	// newVAO->AttributeBinding(3, 4, 3, GL_FLOAT, GL_FALSE, 0);

	VAOs[indexVAO] = newVAO;
}

void WorldChunck::InitInstancedMap(int indexVAO, Map* map)
{
	Buffer* buffer = map->GetMapBuffer();
	VAOs[indexVAO]->BindingBuffer<float>(3, 0, buffer, 3);
	VAOs[indexVAO]->AttributeBinding(3, 3, 3, GL_FLOAT, GL_FALSE, 0);
	VAOs[indexVAO]->BindMapBuffer(3, 1);
}

void WorldChunck::SetInstanceMap(int key, Map* value)
{
	InstancedMaps[key] = value;
}

void WorldChunck::SetCollision(int key, Collision* value)
{
	CollisionObjects[key].push_back(value);
}
void WorldChunck::RemoveCollision(int key, Collision* col)
{
	CollisionObjects[key].remove(col);
}
void WorldChunck::RemoveElement(int key, Cube* cube)
{
	InstancedMaps[key]->RemoveElement(cube);
}
