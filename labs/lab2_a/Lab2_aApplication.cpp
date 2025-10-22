#include "Lab2_aApplication.h"
#include <iostream>

#include "shaders/chessboard_vertex.h"
#include "shaders/chessboard_fragment.h"

// Constructor: Call parent constructor with 800x800 square window
Lab2_aApplication::Lab2_aApplication(const std::string& name, const std::string& version)
    : GLFWApplication(name, version, 800, 800),
    m_selectedX(0),
    m_selectedY(0)
{
}

// Destructor: Cleanup OpenGL resources
Lab2_aApplication::~Lab2_aApplication()
{
    // Smart pointers and class destructors in VAO, VBO, EBO handles cleanup automatically
    if (m_shaderProgram != 0) {
        glDeleteProgram(m_shaderProgram);
    }
}

// Initialize - Setup the chessboard geometry and shaders
unsigned Lab2_aApplication::Init()
{
    // Call parent Init first to setup GLFW, window, OpenGL context)
    if (GLFWApplication::Init() != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    std::cout << "Setting up chessboard..." << std::endl;

    // Generate a 8x8 grid using Geometric tools
    auto vertices = GeometricTools::UnitGridGeometry2D<8, 8>();
    auto indices = GeometricTools::UnitGridTopologyTriangles<8, 8>();

    // Create buffers using smart pointers
    auto gridVertexBuffer = std::make_shared<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));


    std::cout << "Indices count: " << indices.size() << std::endl;
    std::cout << "Expected triangles: " << indices.size() / 3 << std::endl;

    // After creating the index buffer:
    auto gridIndexBuffer = std::make_shared<IndexBuffer>(indices.data(), indices.size());
    std::cout << "IndexBuffer count: " << gridIndexBuffer->GetCount() << std::endl;

    // Define the buffer layout
    auto gridBufferLayout = BufferLayout(
        {{ ShaderDataType::Float2, "position" }}
    );

    gridVertexBuffer->SetLayout(gridBufferLayout);

    // Create and configure vertex array
    m_chessboardVAO = std::make_unique<VertexArray>();
    m_chessboardVAO->AddVertexBuffer(gridVertexBuffer);
    m_chessboardVAO->SetIndexBuffer(gridIndexBuffer);

    // Compile shaders
    m_shaderProgram = CompileShader(chessboardVertexShaderSrc, chessboardFragmentShaderSrc);

    // Debugging
    std::cout << "Chessboard setup complete!" << std::endl;
    std::cout << "Vertices: " << vertices.size() / 2 << std::endl;
    std::cout << "indices: " << indices.size() << std::endl;

    return EXIT_SUCCESS;
}

unsigned Lab2_aApplication::Run()
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

        // clear screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the chessboard
        RenderChessboard();

        glfwSwapBuffers(window);
    }

    std::cout << "Exiting..." << std::endl;
    return EXIT_SUCCESS;
}

void Lab2_aApplication::HandleInput()
{
    GLFWwindow* window = GetWindow();

    // Simple debouncing; track if key was pressed last frame
    static bool keyWasPressed = false;
    bool keyIsPressed = false;

    // Check arrow keys
    // RIGHT
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedX < 7) {
            m_selectedX++;
        }
        keyIsPressed = true;
    }
    // LEFT
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedX > 0) {
            m_selectedX--;
        }
        keyIsPressed = true;
    }
    // UP
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedY < 7) {
            m_selectedY++;
        }
        keyIsPressed = true;
    }
    // DOWN
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedY > 0) {
            m_selectedY--;
        }
        keyIsPressed = true;
    }

    // Update key state for next frame
    keyWasPressed = keyIsPressed;

    // ESC to exit program
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// Render the chessboard
void Lab2_aApplication::RenderChessboard()
{
    glUseProgram(m_shaderProgram);
    m_chessboardVAO->Bind();

    // Pass the selected tile to the shader
    GLint selectedTileLoc = glGetUniformLocation(m_shaderProgram, "u_SelectedTile");
    glUniform2i(selectedTileLoc, m_selectedX, m_selectedY);

    // Draw the chessboard
    glDrawElements(GL_TRIANGLES, m_chessboardVAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, (void*)0);
}