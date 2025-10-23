#ifndef LAB3APPLICATION_H_
#define LAB3APPLICATION_H_

#include "GLFWApplication.h"
#include "GeometricTools.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"

#include <memory>

class Lab3Application : public GLFWApplication
{
public:

    Lab3Application(const std::string& name, const std::string& version);
    ~Lab3Application();

    // Override base class methods
    unsigned Init() override;
    unsigned Run() override;

private:

    std::unique_ptr<VertexArray> m_chessboardVAO;
    std::unique_ptr<Shader> m_shaderProgram;

    // Tile selector state (0-7)
    int m_selectedX = 0;
    int m_selectedY = 0;

    // Private helper methods
    void HandleInput();
    void RenderChessboard();
};
#endif // LAB3APPLICATION_H_