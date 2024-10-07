#version 460 core

layout(location = 0) out vec4 oFragColor;

in vec3 vPosition;
in vec4 vColor;
in vec2 vTextureCoords;
// in vec3 vNormal;

float ambientLight = .1f;

uniform sampler2D gSampler;

uniform float vNormalScale = 1.f;

void main()
{
    
	oFragColor = texture(gSampler, vTextureCoords) * vColor;
}