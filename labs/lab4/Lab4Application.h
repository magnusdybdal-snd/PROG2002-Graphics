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
#include "TextureManager.h"
#include "PerspectiveCamera.h"

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Lab4Application : public GLFWApplication
{
public:

    Lab4Application(const std::string& name, const std::string& version);
    ~Lab4Application();

    // Override base class methods
    unsigned Init() override;
    unsigned Run() override;

private:

    static int constexpr GRID_SIZE = 8;

    std::shared_ptr<VertexArray> m_chessboardVAO;
    std::shared_ptr<VertexArray> m_unitCubeVAO;
    std::unique_ptr<Shader> m_chessboardShaderProgram;
    std::unique_ptr<Shader> m_unitCubeShaderProgram;
    std::unique_ptr<PerspectiveCamera> m_camera;

    glm::mat4 m_chessboardModelMatrix;
    glm::mat4 m_unitCubeModelMatrix;
    glm::vec4 m_cubeBlendColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f); // Default no color

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
    void InitializeTextures();
    void InitializeChessboard();
    void InitializeUnitCube();
    void InitializeShaders();
    void InputHandleCubeRotation(GLFWwindow* window);
    void InputHandleTileSelection(GLFWwindow* window);
    void InputHandleColorBlending(GLFWwindow* window);
};
#endif // LAB4APPLICATION_H_