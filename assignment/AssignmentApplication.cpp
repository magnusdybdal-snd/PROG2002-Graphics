#include "AssignmentApplication.h"

#include <iostream>

#include "shaders/chess_assignment_fragment.h"
#include "shaders/chess_assignment_vertex.h"
#include "shaders/red_cube_fragment.h"
#include "shaders/red_cube_vertex.h"

AssignmentApplication::AssignmentApplication(const std::string &name, const std::string &version)
    : GLFWApplication(name, version, 800, 600),
    m_selectedX(0),
    m_selectedY(0)
{

}

AssignmentApplication::~AssignmentApplication()
{

}

unsigned AssignmentApplication::Init()
{
    // Call parent Init first to setup GLFW, window, OpenGL context)
    if (GLFWApplication::Init() != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    // =============== CAMERA SETUP ===============
    m_camera = std::make_unique<PerspectiveCamera>(
        PerspectiveCamera::Frustrum{CAMERA_FOV, CAMERA_WIDTH, CAMERA_HEIGHT, CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE},
        glm::vec3(0.0f, 0.0f, 5.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    
    InitializeChessboard();
    InitializeChessPieces();
    InitializeShaders();

    return EXIT_SUCCESS;
}

unsigned AssignmentApplication::Run()
{
    // Get the window
    GLFWwindow* window = GetWindow();
    
    // std::cout << "Use arrow keys to move selector, Q to exit" << std::endl;
    
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

void AssignmentApplication::HandleInput()
{
    // Get the window
    GLFWwindow* window = GetWindow();

    InputHandleTileSelection(window);
    // ESC to exit program
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void AssignmentApplication::RenderChessboard()
{
    m_chessboardShaderProgram->Bind();
    m_chessboardVAO->Bind();

    // Pass uniforms to shader
    m_chessboardShaderProgram->UploadUniformMat4("u_ChessboardModelMatrix", m_chessboardModelMatrix);
    m_chessboardShaderProgram->UploadUniformMat4("u_ViewProjectionMatrix", m_camera->GetViewProjectionMatrix());
    m_chessboardShaderProgram->UploadUniformInt2("u_SelectedTile", glm::ivec2(m_selectedX, m_selectedY));
    m_chessboardShaderProgram->UploadUniformInt("u_GridSize",GRID_SIZE);

    // Draw the chessboard
    RenderCommands::DrawIndex(m_chessboardVAO, GL_TRIANGLES);
}

void AssignmentApplication::RenderChessPieces()
{
    m_redCubeShaderProgram->Bind();
    m_chessPiecesVAO->Bind();
    
    // Draw each piece with its own model matrix
    for (const auto& piece : m_chessPieces) {
        m_redCubeShaderProgram->UploadUniformMat4("u_CubeModelMatrix", piece.modelMatrix);
        m_redCubeShaderProgram->UploadUniformMat4("u_ViewProjectionMatrix", 
                                                   m_camera->GetViewProjectionMatrix());
        RenderCommands::DrawIndex(m_chessPiecesVAO, GL_TRIANGLES);
    }
}

void AssignmentApplication::InitializeChessPieces()
{
    // ----------------------------------Geometry Setup---------------------------------------

    // Generate vertices and indices for the cube
    auto redCubeVertices = GeometricTools::UnitCubeGeometry3D;
    auto redCubeIndices = GeometricTools::UnitCubeTopologyTriangles;

    // --------------------------------Model Matrix Setup-------------------------------------

    m_cubeModelMatrix = glm::mat4(1.0f);
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 2; j++){
            PlaceChessPiece(i,j);
        }
    }
    
    // -----------------------------------Buffer Setup & Layout--------------------------------

    // Create vertex and index buffers using smart pointers
    auto cubeVertexBuffer = std::make_shared<VertexBuffer>(redCubeVertices.data(), redCubeVertices.size() * sizeof(float));
    auto cubeIndexBuffer = std::make_shared<IndexBuffer>(redCubeIndices.data(), redCubeIndices.size());

    // Define the buffer layout
    auto redCubeBufferLayout = BufferLayout(
        {{ ShaderDataType::Float3, "cube_position" }}
    );

    // Set the layouts in the vertex buffer
    cubeVertexBuffer->SetLayout(redCubeBufferLayout);

    // ---------------------------------------- VAO Setup -------------------------------------

    m_chessPiecesVAO = std::make_shared<VertexArray>();
    m_chessPiecesVAO->AddVertexBuffer(cubeVertexBuffer);
    m_chessPiecesVAO->SetIndexBuffer(cubeIndexBuffer);


    m_chessPiecesVAO->Unbind();
}

void AssignmentApplication::PlaceChessPiece(int gridX, int gridY)
{
    ChessPiece piece;
    piece.gridX = gridX;
    piece.gridY = gridY;
    piece.position = GetTileWorldPosition(gridX, gridY);
    
    // Create model matrix: scale the piece to reasonable size
    piece.modelMatrix = glm::mat4(1.0f);
    piece.modelMatrix = glm::translate(piece.modelMatrix, piece.position);
    piece.modelMatrix = glm::scale(piece.modelMatrix, glm::vec3(0.2f)); // Adjust size
    
    m_chessPieces.push_back(piece);
}

void AssignmentApplication::InitializeChessboard()
{
    // ----------------------------------Geometry Setup---------------------------------------

    // Generate a 8x8 grid using Geometric tools
    auto vertices = GeometricTools::UnitGridGeometry2D<GRID_SIZE, GRID_SIZE>();
    auto indices = GeometricTools::UnitGridTopologyTriangles<GRID_SIZE, GRID_SIZE>();

    // --------------------------------Model Matrix Setup-------------------------------------

    // Start with identity matrix
    m_chessboardModelMatrix = glm::mat4(1.0f);

    // 1. Scale, upscale by 2
    m_chessboardModelMatrix = glm::scale(
        m_chessboardModelMatrix,                                    // Matrix to transform
        glm::vec3(CHESSBOARD_SCALE, CHESSBOARD_SCALE, 1.0f));       // Scale factors (X, Y, Z)

    // 2. Rotate, 60 degrees by x axis
    m_chessboardModelMatrix = glm::rotate(
        m_chessboardModelMatrix,                                    // Matrix to transform
        glm::radians(CHESSBOARD_TILT_ANGLE),                        // Angle (converts degrees to radians)
        glm::vec3(1.0f, 0.0f, 0.0f));                               // Which axis to rotate (X, Y, Z)

    // 3. Translate (move) - stays at origin
    m_chessboardModelMatrix = glm::translate(
        m_chessboardModelMatrix,                                    // Matrix to transform
        glm::vec3(0.0f, CHESSBOARD_Y_OFFSET, 0.0f));                // Position (X, Y, Z)
    
    // -----------------------------------Buffer Setup & Layout--------------------------------

    // Create vertex and index buffers using smart pointers
    auto chessboardVertexBuffer = std::make_shared<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
    auto chessboardIndexBuffer = std::make_shared<IndexBuffer>(indices.data(), indices.size());

    // Define the buffer layout
    auto chessboardBufferLayout = BufferLayout({
        { ShaderDataType::Float2, "position" }
    }
    );
    // Set the layout in the vertex buffer
    chessboardVertexBuffer->SetLayout(chessboardBufferLayout);

    // ---------------------------------------- VAO Setup -------------------------------------

    // Create and configure vertex array
    m_chessboardVAO = std::make_shared<VertexArray>();
    m_chessboardVAO->AddVertexBuffer(chessboardVertexBuffer);
    m_chessboardVAO->SetIndexBuffer(chessboardIndexBuffer);
    m_chessboardVAO->Unbind();
}

void AssignmentApplication::InitializeShaders()
{   // TODO move to GLFWApplication
    // Create and compile shaders
    m_chessboardShaderProgram = std::make_unique<Shader>(
        chessboardVertexShaderSrc.c_str(), chessboardFragmentShaderSrc.c_str()
    );
    m_redCubeShaderProgram = std::make_unique<Shader>(
        redCubeVertexShaderSrc.c_str(), redCubeFragmentShaderSrc.c_str()
    );
}

void AssignmentApplication::InputHandleTileSelection(GLFWwindow *window)
{
    // Simple debouncing; track if key was pressed last frame
    static bool keyWasPressed = false;
    bool keyIsPressed = false;

    // Check arrow keys
    // RIGHT KEY
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedX < MAX_GRID_INDEX) {
            m_selectedX++;
        }
        keyIsPressed = true;
    }
    // LEFT KEY
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedX > MIN_GRID_INDEX) {
            m_selectedX--;
        }
        keyIsPressed = true;
    }
    // UP KEY
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedY < MAX_GRID_INDEX) {
            m_selectedY++;
        }
        keyIsPressed = true;
    }
    // DOWN KEY
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedY > MIN_GRID_INDEX) {
            m_selectedY--;
        }
        keyIsPressed = true;
    }

    // Update key state for next frame
    keyWasPressed = keyIsPressed;
}  

glm::vec3 AssignmentApplication::GetTileWorldPosition(int gridX, int gridY) {
    // Convert grid coordinates (0-7) to unit grid space (-0.5 to 0.5)
    float normalizedX = (gridX / float(GRID_SIZE)) - 0.5f + (0.5f / GRID_SIZE);
    float normalizedY = (gridY / float(GRID_SIZE)) - 0.5f + (0.5f / GRID_SIZE);
    
    // Create position in grid space (z=0 since chessboard is 2D)
    glm::vec4 gridPosition = glm::vec4(normalizedX, normalizedY, 0.0f, 1.0f);
    
    // Apply chessboard transformation to get world position
    glm::vec4 worldPosition = m_chessboardModelMatrix * gridPosition;
    
    // Offset the piece above the board (adjust height as needed)
    return glm::vec3(worldPosition.x, worldPosition.y, worldPosition.z + 0.0f);
}