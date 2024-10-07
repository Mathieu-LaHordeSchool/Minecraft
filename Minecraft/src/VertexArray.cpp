#include "VertexArray.h"

#include <GL/glew.h>
#include <Buffer.h>

VertexArrayObject::VertexArrayObject()
{
	glCreateVertexArrays(1, &vaoId);
}

VertexArrayObject::~VertexArrayObject() noexcept
{
	glDeleteVertexArrays(1, &vaoId);
}

void VertexArrayObject::AttributeBinding(int bindIndex, int layoutIndex, int size, unsigned int type, unsigned int normalize, int offset)
{
	glEnableVertexArrayAttrib(vaoId, layoutIndex);
	glVertexArrayAttribBinding(vaoId, layoutIndex, bindIndex);
	glVertexArrayAttribFormat(vaoId, layoutIndex, size, type, normalize, offset);
}

void VertexArrayObject::BindVertexArray()
{
	glBindVertexArray(vaoId);
}

void VertexArrayObject::UnbindVertexArray()
{
	glBindVertexArray(0);
}

void VertexArrayObject::BindMapBuffer(int bufferIndex, int div)
{
	glVertexArrayBindingDivisor(vaoId, bufferIndex, div);
}

void VertexArrayObject::BindElementBuffer(Buffer* elementBuffer)
{
	glVertexArrayElementBuffer(vaoId, elementBuffer->GetBuffer());
}
