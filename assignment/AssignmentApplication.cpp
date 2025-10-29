#include "AssignmentApplication.h"

#include <iostream>

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
        glm::vec3(2.5f, -2.5f, 2.0f), // camera position.
        glm::vec3(0.0f, 0.0f, 0.0f), // look at vector.
        glm::vec3(0.0f, 0.0f, 1.0f)  // up-direction.
    );
    
    glEnable(GL_DEPTH_TEST);

    InitializeChessboard();
    InitializeChessPieces();
    InitializeShaders();

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

/**
 * Handles all user input for the application
 * Processes tile selection with arrow keys, piece selection/movement with Enter, and exit with Q
 */
void AssignmentApplication::HandleInput()
{
    // Get the window
    GLFWwindow* window = GetWindow();

    InputHandleTileSelection(window);
    InputHandlePieceSelection(window);
    InputHandleCameraRotation(window);
 //   InputHandleCameraZoom(window);
    // ESC to exit program
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

/**
 * Renders the chessboard
 * Binds the chessboard shader and VAO, uploads necessary uniforms (model matrix, view-projection matrix,
 * selected tile, and grid size), and issues the draw call
 */
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

/**
 * Renders all chess pieces
 * Iterates through all pieces, determines their color based on selection state and position,
 * uploads uniforms, and draws each piece with its own model matrix
 */
void AssignmentApplication::RenderChessPieces()
{
    m_redCubeShaderProgram->Bind();
    m_chessPiecesVAO->Bind();

    for (unsigned int i = 0; i < m_chessPieces.size(); i++){
        const auto& piece = m_chessPieces[i];

        glm::vec3 color;

        // If piece is selected with enter - Yellow
        if (i == m_selectedPieceIndex) {
            color = glm::vec3(1.0f, 1.0f, 0.4f);
        }

        // If tile selector is hovering the piece - Green
        else if (m_selectedX == piece.gridX && m_selectedY == piece.gridY){
            color = glm::vec3(0.0f, 0.9f, 0.0f);
        }

        else {
            color = (i < 16) ? glm::vec3(0.8f, 0.2f, 0.2f) : glm::vec3(0.2f, 0.2f, 0.8f);
        }

        // Draw each piece with its own model matrix
        m_redCubeShaderProgram->UploadUniformFloat3("u_Color", color);
        m_redCubeShaderProgram->UploadUniformMat4("u_CubeModelMatrix", piece.modelMatrix);
        m_redCubeShaderProgram->UploadUniformMat4("u_ViewProjectionMatrix", 
                                                  m_camera->GetViewProjectionMatrix());

        RenderCommands::DrawIndex(m_chessPiecesVAO, GL_TRIANGLES);
    }
}

/**
 * Initializes the chess pieces
 * Creates the cube geometry and buffers, sets up the VAO, and places all 32 pieces
 * in their starting positions (rows 0-1 for red team, rows 6-7 for blue team)
 */
void AssignmentApplication::InitializeChessPieces()
{
    // ----------------------------------Geometry Setup---------------------------------------

    // Generate vertices and indices for the cube
    auto redCubeVertices = GeometricTools::UnitCubeGeometry3D;
    auto redCubeIndices = GeometricTools::UnitCubeTopologyTriangles;

    // --------------------------------Model Matrix Setup-------------------------------------

    m_cubeModelMatrix = glm::mat4(1.0f);
    for (int i = 0; i < GRID_SIZE; i++){
        for (int j = 0; j < 2; j++){
            PlaceChessPiece(i,j);
        }
    }
    for (int i = 0; i < GRID_SIZE; i++){ // x
        for (int j = 6; j < GRID_SIZE; j++){ // y
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

/**
 * Places a chess piece at the specified grid coordinates
 * Creates a ChessPiece with the given position, calculates its world position,
 * builds its model matrix, and adds it to the pieces vector
 * 
 * @param gridX The X coordinate on the grid (0-7)
 * @param gridY The Y coordinate on the grid (0-7)
 */
void AssignmentApplication::PlaceChessPiece(int gridX, int gridY)
{
    ChessPiece piece;
    piece.gridX = gridX;
    piece.gridY = gridY;
    piece.position = GetTileWorldPosition(gridX, gridY);
    
    // Create model matrix: scale the piece to reasonable size
    piece.modelMatrix = glm::mat4(1.0f);
    piece.modelMatrix = glm::translate(piece.modelMatrix, piece.position);
    piece.modelMatrix = glm::scale(piece.modelMatrix, glm::vec3(CHESSPIECE_SCALE)); // Adjust size
    
    m_chessPieces.push_back(piece);
}

/**
 * Initializes the chessboard
 * Generates an 8x8 grid, creates the model matrix with scaling/rotation/translation,
 * sets up vertex and index buffers, and configures the VAO
 */
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

/**
 * Initializes the shader programs
 * Compiles and links the chessboard and chess piece shaders from source
 */
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

/**
 * Handles tile selection input using arrow keys
 * Uses debouncing to prevent multiple triggers from a single key press
 * Updates m_selectedX and m_selectedY based on arrow key input
 * 
 * @param window The GLFW window to poll for input
 */
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

void AssignmentApplication::InputHandleCameraRotation(GLFWwindow *window)
{
    // static bool enterWasPressed = false;
    // bool enterPressed = (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS);
    // bool enterPressed = (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS);

    // m_camera = std::make_unique<PerspectiveCamera>(
    //     PerspectiveCamera::Frustrum{CAMERA_FOV, CAMERA_WIDTH, CAMERA_HEIGHT, CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE},
    //     glm::vec3(2.5f, -2.5f, 2.0f), // camera position.
    //     glm::vec3(0.0f, 0.0f, 0.0f), // look at vector.
    //     glm::vec3(0.0f, 0.0f, 1.0f)  // up-direction.
    // );
    // H = Rotate left
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        m_cameraXPos -= CAMERA_ROTATION_SPEED;
        m_cameraYPos -= CAMERA_ROTATION_SPEED;
    }
    // L = Rotate right
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        m_cameraXPos += CAMERA_ROTATION_SPEED;
        m_cameraYPos += CAMERA_ROTATION_SPEED;
    }
    m_camera->SetPosition(glm::vec3(glm::sin(m_cameraXPos), glm::cos(m_cameraYPos), 2.0f) * glm::vec3(m_cameraZoomValue, m_cameraZoomValue, 1.0f));
}

/**
 * Handles piece selection and movement using the Enter key
 * First Enter press selects a piece at the current tile (if one exists)
 * Second Enter press attempts to move the selected piece to the current tile
 * Movement is blocked if the destination tile is occupied
 * Uses debouncing to prevent multiple triggers from a single key press
 * 
 * @param window The GLFW window to poll for input
 */
void AssignmentApplication::InputHandlePieceSelection(GLFWwindow* window)
{
    static bool enterWasPressed = false;
    bool enterPressed = (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS);
    
    // Edge trigger: only act when Enter is newly pressed
    if (enterPressed && !enterWasPressed) {
        
        // Check that no piece is selected
        if (m_selectedPieceIndex == -1) {
            // === SELECTION PHASE ===
            // Find and select piece at current tile
            int pieceIndex = FindPieceAt(m_selectedX, m_selectedY);
            if (pieceIndex != -1) {
                m_selectedPieceIndex = pieceIndex;
            }
        } 
        else {
            // === MOVEMENT PHASE ===
            auto& piece = m_chessPieces[m_selectedPieceIndex];
            
            // Check if destination has any piece (including ourselves)
            int blockingPiece = FindPieceAt(m_selectedX, m_selectedY);
            
            // Only move if destination is empty
            if (blockingPiece == -1) {
                // Get world position of destination
                glm::vec3 newPosition = GetTileWorldPosition(m_selectedX, m_selectedY);
                
                // Rebuild the matrix of the chesspiece
                piece.modelMatrix = glm::mat4(1.0f);
                piece.modelMatrix = glm::translate(piece.modelMatrix, newPosition);
                piece.modelMatrix = glm::scale(piece.modelMatrix, glm::vec3(CHESSPIECE_SCALE));
                
                // Update chesspiece new position
                piece.position = newPosition;
                piece.gridX = m_selectedX;
                piece.gridY = m_selectedY;
            }
            
            // Always deselect (whether move succeeded or was blocked/cancelled)
            m_selectedPieceIndex = -1;
        }
    }
    
    enterWasPressed = enterPressed;
}

/**
 * Checks if a chess piece is at the given grid position and returns its index
 * 
 * @param gridX The X coordinate on the grid (0-7)
 * @param gridY The Y coordinate on the grid (0-7)
 * 
 * @return The index of the piece at the given position, or -1 if no piece is found
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
 * Converts grid coordinates to world space position
 * Takes grid coordinates (0-7), normalizes them to unit grid space (-0.5 to 0.5),
 * applies the chessboard transformation matrix, and offsets the piece above the board
 * 
 * @param gridX The X coordinate on the grid (0-7)
 * @param gridY The Y coordinate on the grid (0-7)
 * 
 * @return The world space position (vec3) where the piece should be placed
 */
glm::vec3 AssignmentApplication::GetTileWorldPosition(int gridX, int gridY) {
    // Convert grid coordinates (0-7) to unit grid space (-0.5 to 0.5)
    float normalizedX = (gridX / float(GRID_SIZE)) - 0.5f + (0.5f / GRID_SIZE);
    float normalizedY = (gridY / float(GRID_SIZE)) - 0.5f + (0.5f / GRID_SIZE);
    
    // Create position in grid space (z=0 since chessboard is 2D)
    glm::vec4 gridPosition = glm::vec4(normalizedX, normalizedY, 0.0f, 1.0f);
    
    // Apply chessboard transformation to get world position
    glm::vec4 worldPosition = m_chessboardModelMatrix * gridPosition;
    
    // Offset the piece above the board (adjust height as needed)
    return glm::vec3(worldPosition.x, worldPosition.y, worldPosition.z + CHESSPIECE_Y_OFFSET);
}