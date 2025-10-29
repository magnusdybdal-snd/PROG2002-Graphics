#ifndef ASSIGNMENTAPPLICATION_H_
#define ASSIGNMENTAPPLICATION_H_

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

class AssignmentApplication : public GLFWApplication
{
public:

    AssignmentApplication(const std::string& name, const std::string& version);
    ~AssignmentApplication();

    // Override base class methods
    unsigned Init() override;
    unsigned Run() override;

private:

    // ===== GRID CONSTANTS =====
    static constexpr int GRID_SIZE = 8;
    static constexpr int MAX_GRID_INDEX = GRID_SIZE - 1;
    static constexpr int MIN_GRID_INDEX = 0;
    
    // ===== CHESSBOARD CONSTANTS =====
    static constexpr float CHESSBOARD_SCALE = 3.0f;
    static constexpr float CHESSBOARD_TILT_ANGLE = 0.0f;  // degrees
    static constexpr float CHESSBOARD_Y_OFFSET = 0.0f;

    // ===== CHESSPIECES CONSTANTS =====
    static constexpr float CHESSPIECE_SCALE = 0.2f;
    static constexpr float CHESSPIECE_Y_OFFSET = 0.12f;
    
    // ===== CAMERA CONSTANTS =====
    static constexpr float CAMERA_FOV = 45.0f;              // degrees
    static constexpr float CAMERA_WIDTH = 800.0f;
    static constexpr float CAMERA_HEIGHT = 600.0f;
    static constexpr float CAMERA_NEAR_PLANE = 1.0f;
    static constexpr float CAMERA_FAR_PLANE = 10.0f;
    static constexpr float CAMERA_DISTANCE = 5.0f;          // Z distance from origin
    static constexpr float CAMERA_ZOOM_SPEED = 1.5f;
    static constexpr float CAMERA_ROTATION_SPEED = 0.01f;

    // ===== DYNAMIC CAMERA VARIABLES =====
    float m_cameraXPos;
    float m_cameraYPos;
    float m_cameraZoomValue = 3.5f;

    std::shared_ptr<VertexArray> m_chessboardVAO;
    std::shared_ptr<VertexArray> m_chessPiecesVAO;
    std::unique_ptr<Shader> m_chessboardShaderProgram;
    std::unique_ptr<Shader> m_redCubeShaderProgram;
    std::unique_ptr<PerspectiveCamera> m_camera;


    glm::mat4 m_chessboardModelMatrix;
    glm::mat4 m_cubeModelMatrix;

    // Tile selector state (0-7)
    int m_selectedX = 0;
    int m_selectedY = 0;

    // Private helper methods
    void HandleInput();
    void RenderChessboard();

    void RenderChessPieces();
    void InitializeChessPieces();
    void PlaceChessPiece(int gridX, int gridY);

    // void InitializeTextures();
    void InitializeChessboard();
    void InitializeShaders();
    void InputHandleTileSelection(GLFWwindow* window);
    void InputHandleCameraRotation(GLFWwindow* window);
 //   void InputHandleCameraZoom(GLFWwindow* window);
    void InputHandlePieceSelection(GLFWwindow* window);
    glm::vec3 GetTileWorldPosition(int gridX, int gridY);
    int FindPieceAt(int gridX, int gridY) const;

    struct ChessPiece {
        glm::mat4 modelMatrix;
        glm::vec3 position;
        int gridX;
        int gridY;
    };
    int m_selectedPieceIndex = -1;
    std::vector<ChessPiece> m_chessPieces;
};
#endif // AssignmentApplication_H_