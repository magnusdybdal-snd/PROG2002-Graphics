#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaders/triangleshader.h"
#include "shaders/squareshader.h"

#include <iostream>
#include <set>
#include <cmath>

/**
 * FUNCTION PROTOTYPES
 */
GLuint CompileShader(const std::string& vertexShader,
                     const std::string& fragmentShader);

GLuint CreateTriangle();
GLuint CreateSquare();

void CleanVAO(GLuint &vao);
float EaseInOutQuad(float t);

void GLFWErrorCallback(int code, const char* description);
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

  // Creating the square and its associated shader program
  auto squareVAO = CreateSquare();
  auto squareShaderProgram = CompileShader(squareVertexShaderSrc, squareFragmentShaderSrc);

  // Creating the triangle and its associated shader program
  auto triangleVAO = CreateTriangle();
  auto triangleShaderProgram = CompileShader(triangleVertexShaderSrc, triangleFragmentShaderSrc);
  
  double currentTime = 0.0;
  double lastTime = 0.0;
  bool alternate = false;
  glfwSetTime(0.0);

  // MAIN RENDER LOOP
  while (!glfwWindowShouldClose(window)) {
    // Process the event queue
    glfwPollEvents();

    // Handles (swaps) the alternate flag every 1 second
    currentTime = glfwGetTime();
    if (currentTime - lastTime > 1.0) {
      alternate = !alternate;
      lastTime = currentTime;
    }
    
    // Background color values
    float bgR = (sin(currentTime * 0.5f) + 1.0f) * 0.5f;  // oscillates 0 → 1
    float bgG = (cos(currentTime * 0.5f) + 1.0f) * 0.5f;  // oscillates 0 → 1
    float bgB = (sin(currentTime * 1.0f) + 2.0f) * 1.5f;  // oscillates 0 → 1
    
    // Clears screen back to background color
    glClearColor(bgR, bgG, bgB, 0.7f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // ----------DRAW THE SQUARE----------
    // redvalue for square
    auto redValue = EaseInOutQuad(currentTime);

    // Gets the uniform (global) variable u_Color from the square shader program
    auto vertexColorLocation = glGetUniformLocation(squareShaderProgram, "u_Color");

    // Tells openGL to use the square shader program and VAO
    glUseProgram(squareShaderProgram);
    glBindVertexArray(squareVAO);

    // Sets uniform color by passing the red value(changing) and G, B, Alpha
    glUniform4f(vertexColorLocation, redValue, 0.0f, 0.0f, 1.0f);

    // Draws the square using draw elements (indicies) (Shapes, number of indicies, type of indicies, start at)
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);

    // ----------DRAW THE TRIANGLE----------

    // Gets the uniform (global) variable u_AlternateFlag from the triangle shader program
    auto alternateFlagLocation = glGetUniformLocation(triangleShaderProgram, "u_AlternateFlag");

    // Tells openGL to use the triangle shader program and VAO
    glUseProgram(triangleShaderProgram);
    glBindVertexArray(triangleVAO);

    // Updates the uniform (global) variable within the shader program (1 or 0 cast from true/false)
    glUniform1ui(alternateFlagLocation, static_cast<unsigned int>(alternate));

    // Draws the triangle using drawArrays (no indicies)
    glDrawArrays(GL_TRIANGLES, 0, 3);


    glfwSwapBuffers(window);
    // Listen for escape press to exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
  }

  // Cleanup
  glUseProgram(0);    // "use no program" deselects the current shader
  glDeleteProgram(triangleShaderProgram); // Frees the GPU memory from the shader programs
  glDeleteProgram(squareShaderProgram);

  CleanVAO(triangleVAO);
  CleanVAO(squareVAO);

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
  GLfloat squareVerticies[4*3] = {
    -0.5f, -0.5f, 0.0f,   // Bottom left
     0.5f, -0.5f, 0.0f,   // Bottom right
     0.5f,  0.5f, 0.0f,   // Top right
    -0.5f,  0.5f, 0.0f    // Top left
  };

  GLuint squareIndicies[2*3] = {
    0, 1, 2,
    2, 3, 0
  };

  GLuint squareVBO; // Vertex Buffer Object
  GLuint squareEBO; // Element Buffer Object
  GLuint squareVAO; // Vertex Array Object

    // Create a Vertex Array Object (VAO)
  glGenVertexArrays(1, &squareVAO);
  glBindVertexArray(squareVAO);                     // Bind for use

  // Create a Vertex Buffer Object (VBO)
  glGenBuffers(1, &squareVBO);
  glBindBuffer(GL_ARRAY_BUFFER, squareVBO);         // Bind for use
  glBufferData(GL_ARRAY_BUFFER, sizeof(squareVerticies), squareVerticies, GL_STATIC_DRAW); // Populate

  // Create a Element Buffer Object (EBO)
  glGenBuffers(1, &squareEBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEBO); // Bind for use
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndicies), squareIndicies, GL_STATIC_DRAW);// Populate

  // Vertex positions
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  return squareVAO;
}

GLuint CreateTriangle() {

    GLfloat  triangle[3*9] = { // GLfloat has a set size that does not change per system compared to float
    -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 1.0f,
     0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 0.0f
  };

  GLuint triangleVAO;
  GLuint triangleVBO;

  // Create a Vertex Array Object (VAO)
  glGenVertexArrays(1, &triangleVAO);
  glBindVertexArray(triangleVAO);   // Bind it for use

  // Create a Vertext Buffer Object (VBO)
  glGenBuffers(1, &triangleVBO);
  glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);     // Bind it for use

  // Populate the vertex buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

  // Set the layout for vertex positions and enable the attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)0);
  glEnableVertexAttribArray(0);

  // Set the layout for color1 positions 
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Set the layout for color2 positions
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  return triangleVAO;
}

/**
 * Easing function for changing color in square
 */
float EaseInOutQuad(float t) {
    // Normalize to [0,1] over a 2-second cycle
    float x = fmod(t, 4.0f) / 4.0f;

    // Ping-pong: reflect second half
    if (x > 0.5f) {
        x = 1.0f - x;
    }
    x *= 2.0f; // rescale back to [0,1]

    // Ease InOutQuad
    return x < 0.5f
        ? 2.0f * x * x
        : 1.0f - pow(-2.0f * x + 2.0f, 2) / 2.0f;
}

void CleanVAO(GLuint &vao) {
  glBindVertexArray(vao);
  GLint maxVertexAttribs;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
  for (GLint i = 0; i < maxVertexAttribs; i++) {
    glDisableVertexAttribArray(i);
  }
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vao);
  vao = 0;
}

// -----------------------------------------------------------------------------
//  GLFW ERROR CALLBACK
// -----------------------------------------------------------------------------
void GLFWErrorCallback(int code, const char* description)
{
  std::cerr << "GLFW Error " << code << ": " << description << "\n";
}

// -----------------------------------------------------------------------------
//  MESSAGE CALLBACK
// -----------------------------------------------------------------------------
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
