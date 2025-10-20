#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void *vertices, GLsizei size)
{
    glGenBuffers(1, &VertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
    glBufferData(GL_ARRAY_BUFFER,
                 size,
                 vertices,
                 GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{

}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, *vertices);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::BufferSubData(GLintptr offset, GLsizeiptr size, const void *data) const
{

}
