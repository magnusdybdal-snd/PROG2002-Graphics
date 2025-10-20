#include "Lab2Application.h"
#include "GeometricTools.h"
#include <iostream>

// Constructor: Call parent constructor with 800x800 square window
Lab2Application::Lab2Application(const std::string& name, const std::string& version)
    : GLFWApplication(name, version, 800, 800),
    m_selectedX(0),
    m_selectedY(0)
{
}

// Destructor: Cleanup OpenGL resources
Lab2Application::~Lab2Application()
{
    if (m_chessboardVAO != 0) {
        glDeleteVertexArrays(1, &m_chessboardVAO);
    }
    if (m_chessboardVBO != 0) {
        glDeleteBuffers(1, &m_chessboardVBO);
    }
    if (m_chessboardEBO != 0) {
        glDeleteBuffers(1, &m_chessboardEBO);
    }
    if (m_shaderProgram != 0) {
        glDeleteProgram(m_shaderProgram);
    }
}

// Initialize - Setup the chessboard geometry and shaders
unsigned Lab2Application::Init()
{
    // Call parent Init first to setup GLFW, window, OpenGL context)
    if (GLFWApplication::Init() != EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    std::cout << "Setting up chessboard..." << std::endl;

    // Generate a 8x8 grid using Geometric tools
    auto vertices = GeometricTools::UnitGridGeometry2D<8, 8>();
    auto indices = GeometricTools::UnitGridTopologyTriangles<8, 8>();

    // Store index count for rendering
    m_indexCount = static_cast<GLsizei>(indices.size());

    // Create and bind VAO
    glGenVertexArrays(1, &m_chessboardVAO);
    glBindVertexArray(m_chessboardVAO);

    // Create and fill VBO
    glGenBuffers(1, &m_chessboardVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_chessboardVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),
                 vertices.data(),
                 GL_STATIC_DRAW);
    
    // Create and fill the EBO
    glGenBuffers(1, &m_chessboardEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_chessboardEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 indices.data(),
                 GL_STATIC_DRAW);

    // Setup vertex attributes (2D positions)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // TODO: setup shaders
    // m_shaderProgram = CompileShader(vxsrc, fgsrc)

    std::cout << "Chessboard setup complete!" << std::endl;
    std::cout << "Vertices: " << vertices.size() / 2 << std::endl;
    std::cout << "indices: " << indices.size() << std::endl;

    return EXIT_SUCCESS;
}

unsigned Lab2Application::Run()
{
    GLFWwindow* window = GetWindow();

    std::cout << "Starting render loop..." << std::endl;
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

void Lab2Application::HandleInput()
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
            std::cout << "Selevted tile: (" << m_selectedX << ", " << m_selectedY << ")" << std::endl;
        }
        keyIsPressed = true;
    }
    // LEFT
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedX > 0) {
            m_selectedX--;
            std::cout << "Selevted tile: (" << m_selectedX << ", " << m_selectedY << ")" << std::endl;
        }
        keyIsPressed = true;
    }
    // UP
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedY < 7) {
            m_selectedY++;
            std::cout << "Selevted tile: (" << m_selectedX << ", " << m_selectedY << ")" << std::endl;
        }
        keyWasPressed = true;
    }
    // DOWN
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (!keyWasPressed && m_selectedY > 0) {
            m_selectedY--;
            std::cout << "Selevted tile: (" << m_selectedX << ", " << m_selectedY << ")" << std::endl;
        }
        keyWasPressed = true;
    }

    // Update key state for next frame
    keyWasPressed = keyIsPressed;

    // ESC to exit program
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// Render the chessboard
void Lab2Application::RenderChessboard()
{
    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_chessboardVAO);

    // Pass the selected tile to the shader
    GLint selectedTileLoc = glGetUniformLocation(m_shaderProgram, "u_selectedTile");
    glUniform2i(selectedTileLoc, m_selectedX, m_selectedY);

    // Draw the chessboard
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, (void*)0);
}