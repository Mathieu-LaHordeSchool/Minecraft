#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUvTexture;
layout(location = 3) in vec3 aMap;
// layout(location = 4) in vec3 aNormal;

layout(location = 0) uniform mat4 uViewMatrix;
layout(location = 1) uniform mat4 uProjectionMatrix;

out vec4 vColor;
out vec2 vTextureCoords;
out vec3 vPosition;
// out vec3 vNormal;

void main()
{
	vColor = aColor;
	vTextureCoords = aUvTexture;
	// vNormal = aNormal;

	mat4 mvp = uProjectionMatrix * uViewMatrix;
	// vPosition = vec3(mvp * vec4(aPosition + aMap, 1.0));
	gl_Position = mvp * vec4(aPosition + aMap, 1.0);
}