#pragma once

#include <vector>
#include <glm/glm.hpp>

class Buffer;

class Map
{
public:
	explicit Map();
	~Map() noexcept = default;

public:
	inline std::vector<glm::vec3>	GetMapVector()		const { return map;				}
	inline Buffer*					GetMapBuffer()		const { return mapBuffer;		}
	inline int						GetNumInstance()	const { return numInstance;		}

public:
	void AddElement(class Cube* cube);
	void RemoveElement(class Cube* cube);

private:
	Buffer*						mapBuffer;
	std::vector<glm::vec3>		map;
	int							numInstance = 0;
};