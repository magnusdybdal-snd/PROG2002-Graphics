#include "VertexArray.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_vertexArrayID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_vertexArrayID);
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_vertexArrayID);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer)
{
    // Bind the VAO
    glBindVertexArray(m_vertexArrayID);

    // Bind the vertex buffer
    vertexBuffer->Bind();

    // Get the layout
    const BufferLayout &layout = vertexBuffer->GetLayout();

    // Set up vertex attributes based on the layout
    GLuint index = 0;
    for (const auto &attribute : layout)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index,
            ShaderDataTypeComponentCount(attribute.Type),   // Component count
            ShaderDataTypeToOpenGLBaseType(attribute.Type), // Type
            attribute.Normalized,                           // Normalized
            layout.GetStride(),                             // Stride
            (const void*)attribute.Offset                   // Offset
        );
        index++;
    }

    // Store the vertex buffer
    VertexBuffers.push_back(vertexBuffer);



}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer)
{

}
