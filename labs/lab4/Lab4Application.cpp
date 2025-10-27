#include "Lab4Application.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaders/chessboard_vertex.h"
#include "shaders/chessboard_fragment.h"
#include "shaders/unitCube_vertex.h"
#include "shaders/unitCube_fragment.h"

// Constructor: Call parent constructor with 800x800 square window
Lab4Application::Lab4Application(const std::string& name, const std::string& version)
    : GLFWApplication(name, version, 800, 800),
    m_selectedX(0),
    m_selectedY(0)
{
}

// Destructor: Cleanup OpenGL resources
Lab4Application::~Lab4Application()
{
    // Smart pointers and class destructors in VAO, VBO, EBO and Shader handles cleanup automatically
}

// Initialize - Setup the chessboard geometry and shaders
unsigned Lab4Application::Init()
{
    // Call parent Init first to setup GLFW, window, OpenGL context)
    if (GLFWApplication::Init() != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    // =============== OPENGL STATE SETUP ===============

    // Enable depth testing for rendering 3d objects in correct order
    glEnable(GL_DEPTH_TEST);
    // Enable blending
    glEnable(GL_BLEND);
    // Set the blending function: s*alpha + d(1-alpha)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // =============== CAMERA SETUP ===============

    m_camera = std::make_unique<PerspectiveCamera>(
        PerspectiveCamera::Frustrum{CAMERA_FOV, CAMERA_ASPECT_RATIO, CAMERA_ASPECT_RATIO, CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE},
        glm::vec3(0.0f, 0.0f, 5.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // =============== TEXTURE LOADING ===============

    InitializeTextures();
    
    // =============== SCENE SETUP ===============
    
    InitializeChessboard();
    InitializeUnitCube();
    
    // =============== SHADER SETUP ===============

    InitializeShaders();
    
    return EXIT_SUCCESS;
}

unsigned Lab4Application::Run()
{
    GLFWwindow* window = GetWindow();
    
    std::cout << "Use arrow keys to move selector, ESC to exit" << std::endl;
    
    // Main rendering loop
    while (!glfwWindowShouldClose(window)) 
    {
        // Process events
        glfwPollEvents();
        HandleInput();
        UpdateCubeRotation();

        // clear screen
        RenderCommands::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        RenderCommands::Clear();

        // Render the chessboard and cube: Opaque first then transp
        RenderChessboard();
        RenderUnitCube();

        glfwSwapBuffers(window);
    }

    std::cout << "Exiting..." << std::endl;
    return EXIT_SUCCESS;
}

void Lab4Application::HandleInput()
{

    GLFWwindow* window = GetWindow();

    // Input handling seperated into its own helper functions
    InputHandleColorBlending(window);
    InputHandleCubeRotation(window);
    InputHandleTileSelection(window);

    // ESC to exit program
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void Lab4Application::InputHandleTileSelection(GLFWwindow* window)
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

void Lab4Application::InputHandleCubeRotation(GLFWwindow* window)
{
    const float rotationSpeed = CUBE_ROTATION_SPEED;

    // W = Rotate up
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        m_cubeRotationX -= rotationSpeed;
    }
    // S = Rotate down
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        m_cubeRotationX += rotationSpeed;
    }
    // A = Rotate left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        m_cubeRotationY -= rotationSpeed;
    }
    // D = Rotate right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        m_cubeRotationY += rotationSpeed;
    }
}

void Lab4Application::InputHandleColorBlending(GLFWwindow* window) 
{
    static bool colorKeyWasPressed = false;
    bool colorKeyIsPressed = false;

    // 1 = Red
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        if (!colorKeyWasPressed) {
            m_cubeBlendColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); 
        }
        colorKeyIsPressed = true;
    }
    // 2 = Green
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        if (!colorKeyWasPressed) {
            m_cubeBlendColor = glm::vec4(0.2f, 1.2f, 0.2f, 1.0f); 
        }
        colorKeyIsPressed = true;
    }
    // 3 = Blue
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        if (!colorKeyWasPressed) {
            m_cubeBlendColor = glm::vec4(0.2f, 0.2f, 1.0f, 1.0f); 
        }
        colorKeyIsPressed = true;
    }
    // 0 = No color blend
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        if (!colorKeyWasPressed) {
            m_cubeBlendColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); 
        }
        colorKeyIsPressed = true;
    }
    colorKeyWasPressed = colorKeyIsPressed;
}

void Lab4Application::UpdateCubeRotation()
{
    m_unitCubeModelMatrix = glm::mat4(1.0f);

    m_unitCubeModelMatrix = glm::translate(
        m_unitCubeModelMatrix,                          // Matrix to transform
        glm::vec3(0.0f, CUBE_Y_POSITION, 0.0f));        // Position (X, Y, Z)

    m_unitCubeModelMatrix = glm::rotate(m_unitCubeModelMatrix,
                                        glm::radians(m_cubeRotationY),
                                        glm::vec3(0.0f, 1.0f, 0.0f));

    m_unitCubeModelMatrix = glm::rotate(m_unitCubeModelMatrix,
                                        glm::radians(m_cubeRotationX),
                                        glm::vec3(1.0f, 0.0f, 0.0f));
}

// Render the chessboard
void Lab4Application::RenderChessboard()
{
    m_chessboardShaderProgram->Bind();
    m_chessboardVAO->Bind();

    // Pass uniforms to shader
    m_chessboardShaderProgram->UploadUniformMat4("u_viewProjectionMatrix", m_camera->GetViewProjectionMatrix());
    m_chessboardShaderProgram->UploadUniformMat4("u_chessboardModelMatrix", m_chessboardModelMatrix);
    m_chessboardShaderProgram->UploadUniformInt2("u_SelectedTile", glm::ivec2(m_selectedX, m_selectedY));
    m_chessboardShaderProgram->UploadUniformInt("u_Gridsize",GRID_SIZE);

    // Draw the chessboard
    RenderCommands::DrawIndex(m_chessboardVAO, GL_TRIANGLES);
}

// Render the cube
void Lab4Application::RenderUnitCube()
{
    m_unitCubeShaderProgram->Bind();
    m_unitCubeVAO->Bind();

    // Pass uniforms to shader
    m_unitCubeShaderProgram->UploadUniformFloat4("u_cubeBlendColor", m_cubeBlendColor);
    m_unitCubeShaderProgram->UploadUniformMat4("u_viewProjectionMatrix", m_camera->GetViewProjectionMatrix());
    m_unitCubeShaderProgram->UploadUniformMat4("u_unitCubeModelMatrix", m_unitCubeModelMatrix);

    // Draw the cube with texture
    glDisable(GL_DEPTH_TEST);       //glDepthMask(GL_FALSE) If you have multiple overlapping transp objects!!!
    RenderCommands::SetSolidMode();
    RenderCommands::DrawIndex(m_unitCubeVAO, GL_TRIANGLES);
    glEnable(GL_DEPTH_TEST);
}

void Lab4Application::InitializeTextures()
{
    // Creating texture manager and loading textures
    auto textureManager = TextureManager::GetInstance();
    textureManager->LoadTexture2D("floorTexture", std::string(TEXTURES_DIR) + "floor_texture.jpg", 0);
    textureManager->LoadCubeMap("cubeTexture", std::string(TEXTURES_DIR) + "cube_texture2.jpg", 1);
}

void Lab4Application::InitializeChessboard()
{
    // ----------------------------------Geometry Setup---------------------------------------

    // Generate a 8x8 grid using Geometric tools
    auto vertices = GeometricTools::UnitGridGeometry2DWTCoords<GRID_SIZE, GRID_SIZE>();
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
        { ShaderDataType::Float2, "position" },
        { ShaderDataType::Float2, "tcoords" }
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

void Lab4Application::InitializeUnitCube()
{
    // ----------------------------------Geometry Setup---------------------------------------

    // Generate vertices and indices for the cube
    auto UnitCubeVertices = GeometricTools::UnitCubeGeometry3D;
    auto UnitCubeIndices = GeometricTools::UnitCubeTopologyTriangles;

    // --------------------------------Model Matrix Setup-------------------------------------

    m_unitCubeModelMatrix = glm::mat4(1.0f);
    
    // -----------------------------------Buffer Setup & Layout--------------------------------

    // Create vertex and index buffers using smart pointers
    auto cubeVertexBuffer = std::make_shared<VertexBuffer>(UnitCubeVertices.data(), UnitCubeVertices.size() * sizeof(float));
    auto cubeIndexBuffer = std::make_shared<IndexBuffer>(UnitCubeIndices.data(), UnitCubeIndices.size());

    // Define the buffer layout
    auto UnitCubeBufferLayout = BufferLayout(
        {{ ShaderDataType::Float3, "cube_position" }}
    );

    // Set the layouts in the vertex buffer
    cubeVertexBuffer->SetLayout(UnitCubeBufferLayout);

    // ---------------------------------------- VAO Setup -------------------------------------

    m_unitCubeVAO = std::make_shared<VertexArray>();
    m_unitCubeVAO->AddVertexBuffer(cubeVertexBuffer);
    m_unitCubeVAO->SetIndexBuffer(cubeIndexBuffer);
    m_unitCubeVAO->Unbind();
}

void Lab4Application::InitializeShaders()
{
    // Create and compile shaders
    m_chessboardShaderProgram = std::make_unique<Shader>(
        chessboardVertexShaderSrc, chessboardFragmentShaderSrc
    );
    m_unitCubeShaderProgram = std::make_unique<Shader>(
        unitCubeVertexShaderSrc, unitCubeFragmentShaderSrc
    );
}