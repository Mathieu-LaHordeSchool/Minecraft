#include "Render.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <VertexArray.h>
#include <ShaderProgram.h>
#include <Shader.h>
#include <Scene/Scene.h>
#include <Camera.h>
#include <Map.h>
#include <WorldChunck.h>
#include <MinecraftData.h>

Renderer::Renderer()
	: renderData(new RenderObjectData())
{
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glDepthMask(GL_TRUE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Shader* vertexShader   = new Shader("Shaders/VertexShader.vert", EShaderType::Vertex);
	Shader* fragmentShader = new Shader("Shaders/FragmentShader.frag", EShaderType::Fragment);

	shaderProgram = new ShaderProgram(vertexShader, fragmentShader);
}

void Renderer::RenderObjects(Map* map, Mesh* mesh, VertexArrayObject* vao)
{
	glProgramUniformMatrix4fv(shaderProgram->GetShaderProgram(), 0, 1, GL_FALSE, &viewMatrix[0][0]);
	glProgramUniformMatrix4fv(shaderProgram->GetShaderProgram(), 1, 1, GL_FALSE, &projectionMatrix[0][0]);

	if (mesh->texture) {
		mesh->texture->BindTexture(GL_TEXTURE0);

		int textureLocation = glGetUniformLocation(shaderProgram->GetShaderProgram(), "gSampler");
		glUniform1i(textureLocation, GL_TEXTURE0);
	}

	vao->BindVertexArray();
	shaderProgram->StartShaderProgram();

	glDrawElementsInstanced(GL_TRIANGLES, mesh->Vertices.size(), GL_UNSIGNED_INT, nullptr, map->GetNumInstance());

	shaderProgram->StopShaderProgram();
	vao->UnbindVertexArray();
}

void Renderer::RenderScene(Scene* scene, Camera* cam)
{
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewMatrix = glm::lookAt(
		cam->position,
		cam->direction + cam->position,
		glm::vec3(0.f, 1.f, 0.f)
	);

	projectionMatrix = glm::perspective(
		glm::radians(cam->Fov),
		cam->Width / cam->Height,
		cam->zNear, cam->zFar
	);

	for (auto& chk : scene->GetChuncks())
	{
		glm::vec2 chunkPosition = chk->GetChunckPosition() * MC_CHUNk_BLEND_SIZE;
		glm::vec3 worldChunkPosition(
			chunkPosition.x,
			cam->position.y,
			chunkPosition.y
		);

		if (glm::distance(cam->position, worldChunkPosition) > cam->zFar + MC_CHUNk_BLEND_SIZE)
			continue;

		for (auto& [key, map] : chk->GetInstancedMap())
		{
			Mesh* mesh = meshMap[key];
			
			if (map->GetNumInstance() < 1)
				continue;

			RenderObjects(
				map,
				mesh,
				chk->GetVertexArrayObjects()[key]
			);
		}
	}
}

void Renderer::BindMeshIndex(Mesh* mesh, int meshIndex)
{
	meshMap[meshIndex] = mesh;
}
