#include "AssignmentApplication.h"

#include <iostream>
#include <algorithm>

#include "shaders/chess_assignment_fragment.h"
#include "shaders/chess_assignment_vertex.h"
#include "shaders/red_cube_fragment.h"
#include "shaders/red_cube_vertex.h"

/**
 * Constructor for AssignmentApplication
 * Initializes the application with the given name and version, and sets up the window dimensions (800x600)
 * Also initializes the selected tile position to (0,0)
 */
AssignmentApplication::AssignmentApplication(const std::string &name, const std::string &version)
    : GLFWApplication(name, version, 800, 600),
    m_selectedX(0),
    m_selectedY(0)
{
}

/**
 * Destructor for AssignmentApplication
 * Resources are cleaned up by pointers and class destructors in VAO, VBO, EBO and Shaders
 */
AssignmentApplication::~AssignmentApplication()
{
}

/**
 * Initializes the application
 * Sets up the camera with perspective projection, enables depth testing,
 * and initializes the chessboard, chess pieces, and shaders
 * 
 * @return EXIT_SUCCESS if initialization succeeds, EXIT_FAILURE otherwise
 */
unsigned AssignmentApplication::Init()
{
    // Call parent Init first to setup GLFW, window, OpenGL context)
    if (GLFWApplication::Init() != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    // =============== CAMERA SETUP ===============
    m_camera = std::make_unique<PerspectiveCamera>(
        PerspectiveCamera::Frustrum{CAMERA_FOV, CAMERA_WIDTH, CAMERA_HEIGHT, CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE},
        glm::vec3(0.0f, 0.0f, 0.0f), // camera position - Values are updated during render loop.
        glm::vec3(0.0f, 0.0f, 0.0f), // look at vector - camera looks at origin.
        glm::vec3(0.0f, 0.0f, 1.0f)  // up-direction.
    );
    
    glEnable(GL_DEPTH_TEST);

    InitializeChessboard();
    InitializeChessPieces();
    InitializeShaders();
    InitializeTextures();

    return EXIT_SUCCESS;
}

/**
 * Main application loop
 * Continuously renders the scene, processes input, and swaps buffers until the window is closed
 * 
 * @return EXIT_SUCCESS when the application exits normally
 */
unsigned AssignmentApplication::Run()
{
    // Get the window
    GLFWwindow* window = GetWindow();
    
    // Main rendering loop
    while (!glfwWindowShouldClose(window)) 
    {
        // clear screen
        RenderCommands::SetClearColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
        RenderCommands::Clear();

        // Process events
        glfwPollEvents();
        HandleInput();
        RenderChessboard();
        RenderChessPieces();

        glfwSwapBuffers(window);
    }

    std::cout << "Exiting..." << std::endl;
    return EXIT_SUCCESS;
}

/**
 * Initializes the chessboard
 */
void AssignmentApplication::InitializeChessboard()
{
    auto vertices = GeometricTools::UnitGridGeometry2DWTCoords<GRID_SIZE, GRID_SIZE>();
    auto indices = GeometricTools::UnitGridTopologyTriangles<GRID_SIZE, GRID_SIZE>();

    m_chessboardModelMatrix = glm::mat4(1.0f);
    m_chessboardModelMatrix = glm::scale(
        m_chessboardModelMatrix,
        glm::vec3(CHESSBOARD_SCALE, CHESSBOARD_SCALE, 1.0f));
    m_chessboardModelMatrix = glm::rotate(
        m_chessboardModelMatrix,
        glm::radians(CHESSBOARD_TILT_ANGLE),
        glm::vec3(1.0f, 0.0f, 0.0f));
    m_chessboardModelMatrix = glm::translate(
        m_chessboardModelMatrix,
        glm::vec3(0.0f, CHESSBOARD_Y_OFFSET, 0.0f));
    
    auto chessboardVertexBuffer = std::make_shared<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
    auto chessboardIndexBuffer = std::make_shared<IndexBuffer>(indices.data(), indices.size());

    auto chessboardBufferLayout = BufferLayout({
        { ShaderDataType::Float2, "position" },
        { ShaderDataType::Float2, "tCoords" }
    });
    chessboardVertexBuffer->SetLayout(chessboardBufferLayout);

    m_chessboardVAO = std::make_shared<VertexArray>();
    m_chessboardVAO->AddVertexBuffer(chessboardVertexBuffer);
    m_chessboardVAO->SetIndexBuffer(chessboardIndexBuffer);
    m_chessboardVAO->Unbind();
}

/**
 * Initializes the chess pieces
 */
void AssignmentApplication::InitializeChessPieces()
{
    auto redCubeVertices = GeometricTools::UnitCubeGeometry3D;
    auto redCubeIndices = GeometricTools::UnitCubeTopologyTriangles;

    m_cubeModelMatrix = glm::mat4(1.0f);
    for (int i = 0; i < GRID_SIZE; i++){
        for (int j = 0; j < 2; j++){
            PlaceChessPiece(i,j);
        }
    }
    for (int i = 0; i < GRID_SIZE; i++){
        for (int j = 6; j < GRID_SIZE; j++){
            PlaceChessPiece(i,j);
        }
    }
    
    auto cubeVertexBuffer = std::make_shared<VertexBuffer>(redCubeVertices.data(), redCubeVertices.size() * sizeof(float));
    auto cubeIndexBuffer = std::make_shared<IndexBuffer>(redCubeIndices.data(), redCubeIndices.size());
    auto redCubeBufferLayout = BufferLayout(
        {{ ShaderDataType::Float3, "cube_position" }}
    );
    cubeVertexBuffer->SetLayout(redCubeBufferLayout);

    m_chessPiecesVAO = std::make_shared<VertexArray>();
    m_chessPiecesVAO->AddVertexBuffer(cubeVertexBuffer);
    m_chessPiecesVAO->SetIndexBuffer(cubeIndexBuffer);
    m_chessPiecesVAO->Unbind();
}

void AssignmentApplication::InitializeTextures()
{
    auto textureManager = TextureManager::GetInstance();
    textureManager->LoadTexture2D("floorTexture", std::string(TEXTURES_DIR) + "chessboard_textue.jpg", 0);
    textureManager->LoadCubeMap("cubeTexture", std::string(TEXTURES_DIR) + "chesspiece_textue.jpg", 0);
}

/**
 * Initializes the shader programs
 */
void AssignmentApplication::InitializeShaders()
{   
    m_chessboardShaderProgram = std::make_unique<Shader>(
        chessboardVertexShaderSrc.c_str(), chessboardFragmentShaderSrc.c_str()
    );
    m_redCubeShaderProgram = std::make_unique<Shader>(
        redCubeVertexShaderSrc.c_str(), redCubeFragmentShaderSrc.c_str()
    );
}

/**
 * Renders the chessboard
 */
void AssignmentApplication::RenderChessboard()
{
    m_chessboardShaderProgram->Bind();
    m_chessboardVAO->Bind();

    m_chessboardShaderProgram->UploadUniformMat4("u_ChessboardModelMatrix", m_chessboardModelMatrix);
    m_chessboardShaderProgram->UploadUniformMat4("u_ViewProjectionMatrix", m_camera->GetViewProjectionMatrix());
    m_chessboardShaderProgram->UploadUniformInt2("u_SelectedTile", glm::ivec2(m_selectedX, m_selectedY));
    m_chessboardShaderProgram->UploadUniformInt("u_GridSize",GRID_SIZE);
    m_chessboardShaderProgram->UploadUniformInt("u_TextureEnabled",(int)m_textureEnabled);

    RenderCommands::DrawIndex(m_chessboardVAO, GL_TRIANGLES);
}

/**
 * Renders all chess pieces
 */
void AssignmentApplication::RenderChessPieces()
{
    m_redCubeShaderProgram->Bind();
    m_chessPiecesVAO->Bind();

    for (unsigned int i = 0; i < m_chessPieces.size(); i++){
        const auto& piece = m_chessPieces[i];
        glm::vec3 color;

        if (i == m_selectedPieceIndex) {
            color = glm::vec3(1.0f, 1.0f, 0.4f);
        }
        else if (m_selectedX == piece.gridX && m_selectedY == piece.gridY){
            color = glm::vec3(0.0f, 0.9f, 0.0f);
        }
        else {
            color = (i < 16) ? glm::vec3(0.8f, 0.2f, 0.2f) : glm::vec3(0.2f, 0.2f, 0.8f);
        }

        m_redCubeShaderProgram->UploadUniformFloat3("u_Color", color);
        m_redCubeShaderProgram->UploadUniformMat4("u_CubeModelMatrix", piece.modelMatrix);
        m_redCubeShaderProgram->UploadUniformMat4("u_ViewProjectionMatrix", 
                                                  m_camera->GetViewProjectionMatrix());
        //m_redCubeShaderProgram->UploadUniformInt("u_TextureEnabled",(int)m_textureEnabled);

        RenderCommands::DrawIndex(m_chessPiecesVAO, GL_TRIANGLES);
    }
}

/**
 * Handles all user input
 */
void AssignmentApplication::HandleInput()
{
    GLFWwindow* window = GetWindow();

    InputHandleTileSelection(window);
    InputHandlePieceSelection(window);
    InputHandleCameraRotation(window);
    InputHandleCameraZoom(window);
    InputHandleTextureToggle(window);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

/**
 * Handles tile selection input
 */
void AssignmentApplication::InputHandleTileSelection(GLFWwindow *window)
{
    static bool keyWasPressed = false;
    bool keyIsPressed = false;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedX < MAX_GRID_INDEX) m_selectedX++;
        keyIsPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedX > MIN_GRID_INDEX) m_selectedX--;
        keyIsPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedY < MAX_GRID_INDEX) m_selectedY++;
        keyIsPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedY > MIN_GRID_INDEX) m_selectedY--;
        keyIsPressed = true;
    }

    keyWasPressed = keyIsPressed;
}

/**
 * Handles piece selection and movement
 */
void AssignmentApplication::InputHandlePieceSelection(GLFWwindow* window)
{
    static bool enterWasPressed = false;
    bool enterPressed = (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS);
    
    if (enterPressed && !enterWasPressed) {
        if (m_selectedPieceIndex == -1) {
            int pieceIndex = FindPieceAt(m_selectedX, m_selectedY);
            if (pieceIndex != -1) m_selectedPieceIndex = pieceIndex;
        } 
        else {
            auto& piece = m_chessPieces[m_selectedPieceIndex];
            int blockingPiece = FindPieceAt(m_selectedX, m_selectedY);
            if (blockingPiece == -1) {
                glm::vec3 newPosition = GetTileWorldPosition(m_selectedX, m_selectedY);
                piece.modelMatrix = glm::mat4(1.0f);
                piece.modelMatrix = glm::translate(piece.modelMatrix, newPosition);
                piece.modelMatrix = glm::scale(piece.modelMatrix, glm::vec3(CHESSPIECE_SCALE));
                piece.position = newPosition;
                piece.gridX = m_selectedX;
                piece.gridY = m_selectedY;
            }
            m_selectedPieceIndex = -1;
        }
    }
    
    enterWasPressed = enterPressed;
}

/**
 * Handles camera rotation
 */
void AssignmentApplication::InputHandleCameraRotation(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        m_cameraXPos -= CAMERA_ROTATION_SPEED;
        m_cameraYPos -= CAMERA_ROTATION_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        m_cameraXPos += CAMERA_ROTATION_SPEED;
        m_cameraYPos += CAMERA_ROTATION_SPEED;
    }

    float angleX = glm::sin(m_cameraXPos);
    float angleY = -glm::cos(m_cameraXPos);
    float angleZ = 0.45f;

    float x = angleX * m_cameraZoomValue;
    float y = angleY * m_cameraZoomValue;
    float z = angleZ * m_cameraZoomValue;
    
    m_camera->SetPosition(glm::vec3(x, y, z));
}

/**
 * Handles camera zoom
 */
void AssignmentApplication::InputHandleCameraZoom(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        m_cameraZoomValue += CAMERA_ZOOM_SPEED;
        m_cameraZoomValue = std::clamp(m_cameraZoomValue, CAMERA_MAX_ZOOM, CAMERA_MIN_ZOOM);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        m_cameraZoomValue -= CAMERA_ZOOM_SPEED;
        m_cameraZoomValue = std::clamp(m_cameraZoomValue, CAMERA_MAX_ZOOM, CAMERA_MIN_ZOOM);
    }
}

void AssignmentApplication::InputHandleTextureToggle(GLFWwindow *window){
    static bool tWasPressed = false;
    bool tPressed = (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS);
       if (tPressed && !tWasPressed) {
        m_textureEnabled = !m_textureEnabled;
    } 
    tWasPressed = tPressed;
}

/**
 * Finds piece at grid coordinate
 */
int AssignmentApplication::FindPieceAt(int gridX, int gridY) const 
{
    for (size_t i = 0; i < m_chessPieces.size(); ++i) {
        if (m_chessPieces[i].gridX == gridX && 
            m_chessPieces[i].gridY == gridY) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

/**
 * Places a chess piece at grid coordinate
 */
void AssignmentApplication::PlaceChessPiece(int gridX, int gridY)
{
    ChessPiece piece;
    piece.gridX = gridX;
    piece.gridY = gridY;
    piece.position = GetTileWorldPosition(gridX, gridY);
    piece.modelMatrix = glm::mat4(1.0f);
    piece.modelMatrix = glm::translate(piece.modelMatrix, piece.position);
    piece.modelMatrix = glm::scale(piece.modelMatrix, glm::vec3(CHESSPIECE_SCALE));
    m_chessPieces.push_back(piece);
}

/**
 * Converts grid to world position
 */
glm::vec3 AssignmentApplication::GetTileWorldPosition(int gridX, int gridY)
{
    float normalizedX = (gridX / float(GRID_SIZE)) - 0.5f + (0.5f / GRID_SIZE);
    float normalizedY = (gridY / float(GRID_SIZE)) - 0.5f + (0.5f / GRID_SIZE);
    glm::vec4 gridPosition = glm::vec4(normalizedX, normalizedY, 0.0f, 1.0f);
    glm::vec4 worldPosition = m_chessboardModelMatrix * gridPosition;
    return glm::vec3(worldPosition.x, worldPosition.y, worldPosition.z + CHESSPIECE_Y_OFFSET);
}