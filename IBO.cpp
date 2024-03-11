#include "IBO.h"

IBO::IBO(GLuint* indices, GLsizeiptr size)
{
	glGenBuffers(1, &ID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void IBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void IBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IBO::Delete()
{
	glDeleteBuffers(1, &ID);
}