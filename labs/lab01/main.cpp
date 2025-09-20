#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main(void) {

  // Stops the program if GLFW fails
  if(!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    std::cin.get();
    return EXIT_FAILURE;
  }

  // Window properties
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create the window
  auto window = glfwCreateWindow(800, 600, "Issue 3 - Triangle", nullptr, nullptr);
  // Handles if window is not created properly
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    std::cin.get();
    return EXIT_FAILURE;
  }

  // Set the current context
  glfwMakeContextCurrent(window);

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD." << std::endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  // Printing OpenGL information
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";

  // OpenGL Setup
  glClearColor(0.5f, 0.0f, 0.0f, 1.0f);

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

  // Compile the vertex shader
  auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const GLchar* vss = vertexShaderSrc.c_str();
  glShaderSource(vertexShader, 1, &vss, nullptr);
  glCompileShader(vertexShader);

  // Compile the fragment shader
  auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const GLchar* fss = fragmentShaderSrc.c_str();
  glShaderSource(fragmentShader, 1, &fss, nullptr);
  glCompileShader(fragmentShader);

  // Create a shader program
  auto shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Shader objects can be deleted once they have been linked to a shader program
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Activate the program
  glUseProgram(shaderProgram);

  // MAIN RENDER LOOP
  while (!glfwWindowShouldClose(window)) {
    // Process the event queue
    glfwPollEvents();
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT);
    // Draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // Display the scene
    glfwSwapBuffers(window);
    // Listen for escape press to exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
  }

  // Cleanup
  glDisableVertexAttribArray(0);
  glDeleteBuffers(1, &vertexBufferId);
  glDeleteVertexArrays(1, &vertexArrayId);

  glfwTerminate();

  return EXIT_SUCCESS;
}