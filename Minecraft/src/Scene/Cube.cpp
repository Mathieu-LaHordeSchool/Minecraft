#include "Scene/Cube.h"

#include <glm/glm.hpp>
#include <Mesh.h>
#include <WorldChunck.h>

Cube::Cube(glm::vec3 position, int layer, WorldChunck* chk)
	: position(position)
	, layer(layer)
	, chk(chk)
{
}
