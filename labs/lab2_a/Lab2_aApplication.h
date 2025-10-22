#ifndef LAB2_AAPPLICATION_H_
#define LAB2_AAPPLICATION_H_

#include "GLFWApplication.h"
#include "GeometricTools.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BufferLayout.h"
#include "VertexArray.h"

#include <memory>

class Lab2_aApplication : public GLFWApplication
{
public:

    Lab2_aApplication(const std::string& name, const std::string& version);
    ~Lab2_aApplication();

    // Override base class methods
    unsigned Init() override;
    unsigned Run() override;

private:

    std::unique_ptr<VertexArray> m_chessboardVAO;
    GLuint m_shaderProgram = 0;

    // Tile selector state (0-7)
    int m_selectedX = 0;
    int m_selectedY = 0;

    // Private helper methods
    void HandleInput();
    void RenderChessboard();
};
#endif // LAB2_AAPPLICATION_H_