#pragma once

#include <GLFWApplication.h>
#include <GeometricTools.h>
#include <VertexBuffer.h>
#include <IndexBuffer.h>

class Lab2_aApplication : public GLFWApplication
{
public:

    Lab2_aApplication(const std::string& name, const std::string& version);
    ~Lab2_aApplication();

    // Override base class methods
    unsigned Init() override;
    unsigned Run() override;

private:

    GLuint m_chessboardVAO = 0;
    VertexBuffer* m_chessboardVBO = nullptr;
    IndexBuffer* m_chessboardEBO = nullptr;
    GLuint m_shaderProgram = 0;

    // Tile selector state (0-7)
    int m_selectedX = 0;
    int m_selectedY = 0;

    GLsizei m_indexCount = 0;

    // Private helper methods
    void HandleInput();
    void RenderChessboard();
};
