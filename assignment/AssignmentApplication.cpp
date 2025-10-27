#include "AssignmentApplication.h"
#include <iostream>

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
