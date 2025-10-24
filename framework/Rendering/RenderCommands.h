#ifndef RENDERCOMMANDS_H_
#define RENDERCOMMANDS_H_

#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "VertexArray.h"

namespace RenderCommands
{
    inline void Clear(GLuint mode = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    {
        glClear(mode);
    }

    inline void SetPolygonMode(GLenum face, GLenum mode)
    {
        glPolygonMode(face, mode);
    }

    inline void DrawIndex(const std::shared_ptr<VertexArray>& vao, GLenum primitive)
    {
        glDrawElements(primitive, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    inline void SetClearColor(glm::vec4 color)
    {
        glClearColor(color.x, color.y, color.z, color.w);
    }

    inline void SetWireframeMode()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    inline void SetSolidMode()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

#endif // RENDERCOMMANDS_H_