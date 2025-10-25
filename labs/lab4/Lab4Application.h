#ifndef LAB4APPLICATION_H_
#define LAB4APPLICATION_H_

#include "GLFWApplication.h"
#include "GeometricTools.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommands.h"

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

class Lab4Application : public GLFWApplication
{
public:

    Lab4Application(const std::string& name, const std::string& version);
    ~Lab4Application();

    // Override base class methods
    unsigned Init() override;
    unsigned Run() override;

private:

    std::shared_ptr<VertexArray> m_chessboardVAO;
    std::shared_ptr<VertexArray> m_unitCubeVAO;
    std::unique_ptr<Shader> m_chessboardShaderProgram;
    std::unique_ptr<Shader> m_unitCubeShaderProgram;

    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_chessboardModelMatrix;
    glm::mat4 m_unitCubeModelMatrix;

    float m_cubeRotationX = 0.0f;
    float m_cubeRotationY = 0.0f;

    // Tile selector state (0-7)
    int m_selectedX = 0;
    int m_selectedY = 0;

    // Private helper methods
    void HandleInput();
    void RenderChessboard();
    void RenderUnitCube();
    void UpdateCubeRotation();
};
#endif // LAB4APPLICATION_H_