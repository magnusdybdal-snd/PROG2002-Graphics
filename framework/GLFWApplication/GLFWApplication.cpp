#include "GLFWApplication.h"
#include <iostream>

GLFWApplication::GLFWApplication(const std::string &name, const std::string &version)
    :m_name(name), m_version(version)
{
}

GLFWApplication::~GLFWApplication()
{
    if (m_window) {
        glfwDestroyWindow(m_window);
    }
    glfwTerminate();
}

unsigned GLFWApplication::Init()
{
    std::cout << "Initializing " << m_name << " version: " << m_version << std::endl;

    // Setting an error callback for GLFW to capture issues
    glfwSetErrorCallback(GLFWErrorCallback);
    // Stops the program if GLFW fails
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        std::cin.get();
        return EXIT_FAILURE;
    }

    // Window properties
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

    // Create the window
    auto window = glfwCreateWindow(1200, 1200, "Issue 3 - Triangle/Square", nullptr, nullptr);
    // Handles if window is not created properly
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        std::cin.get();
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Set the current context
    glfwMakeContextCurrent(window);

    // Initialize GLADGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Enable capture of debug output to get messages about potential issues
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, 0);
    glDebugMessageControl(GLFW_DONT_CARE, GLFW_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    // Printing OpenGL information
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";

    return EXIT_SUCCESS; 
}

GLuint GLFWApplication::CompileShader(const std::string& vertexShaderSrc,
                                      const std::string& fragmentShaderSrc) 
{
    // Converts the shader source code from std::string to raw char pointer
    auto vertexSrc = vertexShaderSrc.c_str();
    auto fragmentSrc = fragmentShaderSrc.c_str();

    // Compile vertex shader
    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
    glCompileShader(vertexShader);

    // Compile fragment shader
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
    glCompileShader(fragmentShader);

    // Create a shader program
    auto shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Shader objects can be deleted when linked to a program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

  return shaderProgram;
}

// Error callbacks
void GLFWApplication::GLFWErrorCallback(int code, const char* description)
{
    std::cerr << "GLFW Error " << code << ": " << description << "\n";
}

void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam)
{
    // Outputs debugging messages from OpenGL
    std::cerr << "GL CALLBACK: "
        << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
        << " type = 0x" << type
        << ", severity = 0x" << severity
        << ", message = " << message << "\n";
}
