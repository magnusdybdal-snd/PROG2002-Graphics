#include "Lab4Application.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaders/chessboard_vertex.h"
#include "shaders/chessboard_fragment.h"
#include "shaders/unitCube_vertex.h"
#include "shaders/unitCube_fragment.h"

// Image data: 3 channels RGB, 24 bpp


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

    glEnable(GL_DEPTH_TEST);

    // Projection matrix
    m_projectionMatrix = glm::perspective(
        glm::radians(45.0f),    // FOV
        1.0f,                   // Aspect ratio
        1.0f,                   // Near plane
        10.0f                   // Far plane
    );

    std::cout << "ProjectionMatrix created..." << std::endl;

    // ViewMatrix
    m_viewMatrix = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 5.0f),    // Camera position
        glm::vec3(0.0f, 0.0f, 0.0f),    // 
        glm::vec3(0.0f, 1.0f, 0.0f)     // Up vector
    );

    std::cout << "viewMatrix created..." << std::endl;

    // Start with the id matrix
    m_unitCubeModelMatrix = glm::mat4(1.0f);

    m_chessboardModelMatrix = glm::mat4(1.0f);

    // 1. Scale, upscale by 2
    m_chessboardModelMatrix = glm::scale(
        m_chessboardModelMatrix,            // Matrix to transform
        glm::vec3(3.0f, 3.0f, 1.0f));       // Scale factors (X, Y, Z)

    // 2. Rotate, 60 degrees by x axis
    m_chessboardModelMatrix = glm::rotate(
        m_chessboardModelMatrix,            // Matrix to transform
        glm::radians(-80.0f),               // Angle (converts degrees to radians)
        glm::vec3(1.0f, 0.0f, 0.0f));       // Which axis to rotate (X, Y, Z)

    // 3. Translate (move) - stays at origin
    m_chessboardModelMatrix = glm::translate(
        m_chessboardModelMatrix,            // Matrix to transform
        glm::vec3(0.0f, -1.3f, 0.0f));       // Position (X, Y, Z)

    std::cout << "Model matrix created..." << std::endl;

    std::cout << "Setting up chessboard..." << std::endl;

    // Generate a 8x8 grid using Geometric tools
    auto vertices = GeometricTools::UnitGridGeometry2D<8, 8>();
    auto indices = GeometricTools::UnitGridTopologyTriangles<8, 8>();

    auto UnitCubeVertices = GeometricTools::UnitCubeGeometry3D;
    auto UnitCubeIndices = GeometricTools::UnitCubeTopologyTriangles;

    // Create vertex buffers using smart pointers
    auto gridVertexBuffer = std::make_shared<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
    auto UnitCubeVertexBuffer = std::make_shared<VertexBuffer>(UnitCubeVertices.data(), UnitCubeVertices.size() * sizeof(float));

    // Creating the index buffers using smart pointers:
    auto gridIndexBuffer = std::make_shared<IndexBuffer>(indices.data(), indices.size());
    auto UnitCubeIndexBuffer = std::make_shared<IndexBuffer>(UnitCubeIndices.data(), UnitCubeIndices.size());

    // Define the buffer layouts
    auto gridBufferLayout = BufferLayout(
        {{ ShaderDataType::Float2, "position" }}
    );
    auto UnitCubeBufferLayout = BufferLayout(
        {{ ShaderDataType::Float3, "cube_position" }}
    );

    // Set the layouts in the vertex buffer
    gridVertexBuffer->SetLayout(gridBufferLayout);
    UnitCubeVertexBuffer->SetLayout(UnitCubeBufferLayout);

    // Create and configure vertex arrays
    m_chessboardVAO = std::make_shared<VertexArray>();
    m_chessboardVAO->AddVertexBuffer(gridVertexBuffer);
    m_chessboardVAO->SetIndexBuffer(gridIndexBuffer);

    m_unitCubeVAO = std::make_shared<VertexArray>();
    m_unitCubeVAO->AddVertexBuffer(UnitCubeVertexBuffer);
    m_unitCubeVAO->SetIndexBuffer(UnitCubeIndexBuffer);

    // Create and compile shaders
    m_chessboardShaderProgram = std::make_unique<Shader>(chessboardVertexShaderSrc, chessboardFragmentShaderSrc);
    m_unitCubeShaderProgram = std::make_unique<Shader>(unitCubeVertexShaderSrc, unitCubeFragmentShaderSrc);

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

        // Handle user input
        HandleInput();

        UpdateCubeRotation();

        // clear screen
        RenderCommands::SetClearColor(glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));
        RenderCommands::Clear();

        // Render the chessboard and cube
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

    // ===== CHESSBOARD SELECTED TILE CONTROLS =====

    // Simple debouncing; track if key was pressed last frame
    static bool keyWasPressed = false;
    bool keyIsPressed = false;

    // Check arrow keys
    // RIGHT KEY
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedX < 7) {
            m_selectedX++;
        }
        keyIsPressed = true;
    }
    // LEFT KEY
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedX > 0) {
            m_selectedX--;
        }
        keyIsPressed = true;
    }
    // UP KEY
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedY < 7) {
            m_selectedY++;
        }
        keyIsPressed = true;
    }
    // DOWN KEY
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedY > 0) {
            m_selectedY--;
        }
        keyIsPressed = true;
    }

    // Update key state for next frame
    keyWasPressed = keyIsPressed;
    
    // ===== CUBE ROTATION CONTROLS =====

    const float rotationSpeed = 1.5f;

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

    // ESC to exit program
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void Lab4Application::UpdateCubeRotation()
{
    m_unitCubeModelMatrix = glm::mat4(1.0f);

    m_unitCubeModelMatrix = glm::translate(
        m_unitCubeModelMatrix,               // Matrix to transform
        glm::vec3(0.0f, 0.5f, 0.0f));       // Position (X, Y, Z)

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
    m_chessboardShaderProgram->UploadUniformMat4("u_projectionMatrix", m_projectionMatrix);
    m_chessboardShaderProgram->UploadUniformMat4("u_viewMatrix", m_viewMatrix);
    m_chessboardShaderProgram->UploadUniformMat4("u_chessboardModelMatrix", m_chessboardModelMatrix);
    m_chessboardShaderProgram->UploadUniformInt2("u_SelectedTile", glm::ivec2(m_selectedX, m_selectedY));

    // Draw the chessboard
    RenderCommands::DrawIndex(m_chessboardVAO, GL_TRIANGLES);
}

// Render the cube
void Lab4Application::RenderUnitCube()
{
    m_unitCubeShaderProgram->Bind();
    m_unitCubeVAO->Bind();

    // Pass uniforms to shader
    m_unitCubeShaderProgram->UploadUniformMat4("u_projectionMatrix", m_projectionMatrix);
    m_unitCubeShaderProgram->UploadUniformMat4("u_viewMatrix", m_viewMatrix);
    m_unitCubeShaderProgram->UploadUniformMat4("u_unitCubeModelMatrix", m_unitCubeModelMatrix);

    // Draw the cube in solid color
    RenderCommands::SetSolidMode();
    m_unitCubeShaderProgram->UploadUniformFloat4("u_Color", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
    RenderCommands::DrawIndex(m_unitCubeVAO, GL_TRIANGLES);

    // Draw the wireframe of the cube
    RenderCommands::SetLineWidth(3.0f);
    RenderCommands::SetWireframeMode();
    m_unitCubeShaderProgram->UploadUniformFloat4("u_Color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    RenderCommands::DrawIndex(m_unitCubeVAO, GL_TRIANGLES);
    RenderCommands::SetSolidMode();
    RenderCommands::SetLineWidth(1.0f);

}