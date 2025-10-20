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


}