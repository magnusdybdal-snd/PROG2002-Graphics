#include "AssignmentApplication.h"

#include <iostream>

#include "chess_assignment_fragment.h"
#include "chess_assignment_vertex.h"

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

    InitializeChessboard();

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
        // Process events
        glfwPollEvents();
        HandleInput();

        // clear screen
        RenderCommands::SetClearColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
        RenderCommands::Clear();

        glfwSwapBuffers(window);
    }

    std::cout << "Exiting..." << std::endl;
    return EXIT_SUCCESS;
}

void AssignmentApplication::HandleInput()
{
    // Get the window
    GLFWwindow* window = GetWindow();

    // ESC to exit program
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void AssignmentApplication::RenderChessboard()
{

}

void AssignmentApplication::InitializeChessboard()
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
{
    // Create and compile shaders
    m_chessboardShaderProgram = std::make_unique<Shader>(
        chessboardVertexShaderSrc, chessboardFragmentShaderSrc
    );
}
