#pragma once

#include <GLFWApplication.h>
#include <GeometricTools.h>

class Lab2Application : public GLFWApplication
{
public:

    Lab2Application(const std::string& name, const std::string& version);
    ~Lab2Application();

    // Override base class methods
    unsigned Init() override;
    unsigned Run() override;

private:

    GLuint m_chessboardVAO = 0;
    GLuint m_chessboardVBO = 0;
    GLuint m_chessboardEBO = 0;
    GLuint m_shaderProgram = 0;

    // Tile selector state (0-7)
    int m_selectedX = 0;
    int m_selectedY = 0;

    GLsizei m_indexCount = 0;

    // Private helper methods
    void HandleInput();
    void RenderChessboard();
};
