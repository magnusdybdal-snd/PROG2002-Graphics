#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

/**
 * FUNCTION PROTOTYPES
 */
GLuint CompileShader(const std::string& vertexShader,
                     const std::string& fragmentShader);

GLuint CreateTriangle();
GLuint CreateSquare();

void CleanVAO(GLuint &vao);

void GLFWErrorCallBack(int code, const char* description);
void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam);

/**
 * ENTRY POINT
 */
int main(void) {

  // Shader setup

  // Vertex shader that forwards the coordinates
  const std::string vertexShaderSrc = R"(
  #version 430 core
  
  layout(location = 0) in vec2 position;
  
  void main()
  {
    gl_Position = vec4(position, 0.0, 1.0); // Homogeneous coordinates 3D + 1
  }
  )";

  // Fragment shader that fills the triangle with color
  const std::string fragmentShaderSrc = R"(
  #version 430 core
  
  out vec4 color;
  void main()
  {
    color = vec4(1.0, 1.0, 1.0, 1.0);
  }
  )";

  static const std::string squareVertexShaderSrc = R"(
  #version 430 core

  layout(location = 0) in vec3 a_Position;

  void main()
  {
  gl_Position = vec4(a_Position, 1.0f);
  }
  )";


  // Setting an error callback for GLFW to capture issues
  // glfwSetErrorCallback(GLFWErrorCallBack);
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
  //glDebugMessageCallback(MessageCallback, 0);
  glDebugMessageControl(GLFW_DONT_CARE, GLFW_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

  // Printing OpenGL information
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";

  // Creating the square and its associated shader program
  auto squareVAO = CreateSquare();
  auto squareShaderProgram = CompileShader(squareVertexShaderSrc, fragmentShaderSrc);

  auto triangleVAO = CreateTriangle();
  auto triangleShaderProgram = CompileShader(vertexShaderSrc, fragmentShaderSrc);
  
  // Setting background color
  glClearColor(0.5f, 0.0f, 0.0f, 1.0f);

  // MAIN RENDER LOOP
  while (!glfwWindowShouldClose(window)) {
    // Process the event queue
    glfwPollEvents();
    //glUseProgram(squareShaderProgram);
    //glBindVertexArray(squareVAO);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glUseProgram(triangleShaderProgram);
    glBindVertexArray(triangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
    // Listen for escape press to exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
  }

  glfwTerminate();

  return EXIT_SUCCESS;
}

GLuint CompileShader(const std::string& vertexShaderSrc,
                     const std::string& fragmentShaderSrc) {

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

GLuint CreateSquare() {
  GLfloat square[6*2] = {
    -0.5f, -0.5f,   // Bottom left
     0.5f, -0.5f,   // Bottom right
     0.5f,  0.5f,   // Top right
    -0.5f, -0.5f,   // Bottom left
     0.5f,  0.5f,   // Top right
    -0.5f,  0.5f    // Top left
  };

    // Create a Vertex Array Object (VAO)
  GLuint vertexArrayId;   // Buffer to hold state needed to supply vertex data
  glGenVertexArrays(1, &vertexArrayId);
  glBindVertexArray(vertexArrayId);   // Bind it for use

  // Create a Vertext Buffer Object (VBO)
  GLuint vertexBufferId;
  glGenBuffers(1, &vertexBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);     // Bind it for use

  // Populate the vertex buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

  // Set the layout of the bound buffer
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, nullptr);
  // Enables the attributes
  glEnableVertexAttribArray(0);

  return vertexArrayId;
}

GLuint CreateTriangle() {

    GLfloat  triangle[3*2] = { // GLfloat has a set size that does not change per system compared to float
    -0.5f, -0.5f,
    0.5f, -0.5f,
    0.0f,  0.5f
  };

  // Create a Vertex Array Object (VAO)
  GLuint vertexArrayId;   // Buffer to hold state needed to supply vertex data
  glGenVertexArrays(1, &vertexArrayId);
  glBindVertexArray(vertexArrayId);   // Bind it for use

  // Create a Vertext Buffer Object (VBO)
  GLuint vertexBufferId;
  glGenBuffers(1, &vertexBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);     // Bind it for use

  // Populate the vertex buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

  // Set the layout of the bound buffer
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, nullptr);
  // Enables the attributes
  glEnableVertexAttribArray(0);

  return vertexArrayId;
}