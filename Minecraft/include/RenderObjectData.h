#pragma once

#include <Buffer.h>
#include <VertexArray.h>

#include <vector>
#include <map>

class RenderObjectData
{

public:
	Buffer* GetOrCreateVertexBuffer(std::vector<float> data);
	Buffer* GetOrCreateColorBuffer(std::vector<float> data);
	Buffer* GetOrCreateTextureBuffer(std::vector<float> data);
	Buffer* GetOrCreateElementBuffer(std::vector<uint32_t> data);
	Buffer* GetOrCreateNormalBuffer(std::vector<float> data);

private:
	std::map<std::vector<float>, Buffer*>		vertexBuffers;
	std::map<std::vector<float>, Buffer*>		colorBuffers;
	std::map<std::vector<float>, Buffer*>		textureCoordBuffers;
	std::map<std::vector<float>, Buffer*>		normalBuffer;
	std::map<std::vector<uint32_t>, Buffer*>	elementBuffers;
};
