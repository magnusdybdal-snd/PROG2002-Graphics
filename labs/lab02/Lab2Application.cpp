#include "Lab2Application.h"
#include "shaders/triangleshader.h"
#include "shaders/squareshader.h"
#include <iostream>
#include <cmath>

Lab2Application::Lab2Application(const std::string& name, const std::string& version)
    : GLFWApplication(name, version)
{
}

Lab2Application::~Lab2Application()
{
    // Cleanup
    glUseProgram(0);
    glDeleteProgram(m_triangleShaderProgram);
    glDeleteProgram(m_squareShaderProgram);
    
    CleanVAO(m_triangleVAO);
    CleanVAO(m_squareVAO);
}

unsigned Lab2Application::Run()
{
    // Create your geometry and shaders - same as your main()
    m_squareVAO = CreateSquare();
    m_squareShaderProgram = CompileShader(squareVertexShaderSrc, squareFragmentShaderSrc);

    m_triangleVAO = CreateTriangle();
    m_triangleShaderProgram = CompileShader(triangleVertexShaderSrc, triangleFragmentShaderSrc);
    
    double currentTime = 0.0;
    double lastTime = 0.0;
    bool alternate = false;
    glfwSetTime(0.0);

    // MAIN RENDER LOOP - exactly like your current one
    while (!glfwWindowShouldClose(GetWindow())) {
        glfwPollEvents();

        // Handle alternating flag every 1 second
        currentTime = glfwGetTime();
        if (currentTime - lastTime > 1.0) {
            alternate = !alternate;
            lastTime = currentTime;
        }
        
        // Background color animation
        float bgR = (sin(currentTime * 0.5f) + 1.0f) * 0.5f;
        float bgG = (cos(currentTime * 0.5f) + 1.0f) * 0.5f;
        float bgB = (sin(currentTime * 1.0f) + 2.0f) * 1.5f;
        
        glClearColor(bgR, bgG, bgB, 0.7f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // ---------- DRAW THE SQUARE ----------
        auto redValue = EaseInOutQuad(currentTime);
        auto vertexColorLocation = glGetUniformLocation(m_squareShaderProgram, "u_Color");

        glUseProgram(m_squareShaderProgram);
        glBindVertexArray(m_squareVAO);
        glUniform4f(vertexColorLocation, redValue, 0.0f, 0.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);

        // ---------- DRAW THE TRIANGLE ----------
        auto alternateFlagLocation = glGetUniformLocation(m_triangleShaderProgram, "u_AlternateFlag");

        glUseProgram(m_triangleShaderProgram);
        glBindVertexArray(m_triangleVAO);
        glUniform1ui(alternateFlagLocation, static_cast<unsigned int>(alternate));
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(GetWindow());
        
        // Listen for escape to exit
        if (glfwGetKey(GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
    }

    return EXIT_SUCCESS;
}

// Your exact same functions, just moved into the class
GLuint Lab2Application::CreateSquare() 
{
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

    GLuint squareVBO;
    GLuint squareEBO;
    GLuint squareVAO;

    glGenVertexArrays(1, &squareVAO);
    glBindVertexArray(squareVAO);

    glGenBuffers(1, &squareVBO);
    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVerticies), squareVerticies, GL_STATIC_DRAW);

    glGenBuffers(1, &squareEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndicies), squareIndicies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return squareVAO;
}

GLuint Lab2Application::CreateTriangle() 
{
    GLfloat triangle[3*9] = {
        -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 1.0f,
         0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, 0.0f
    };

    GLuint triangleVAO;
    GLuint triangleVBO;

    glGenVertexArrays(1, &triangleVAO);
    glBindVertexArray(triangleVAO);

    glGenBuffers(1, &triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return triangleVAO;
}

float Lab2Application::EaseInOutQuad(float t) 
{
    // Your exact same easing function
    float x = fmod(t, 4.0f) / 4.0f;
    if (x > 0.5f) {
        x = 1.0f - x;
    }
    x *= 2.0f;

    return x < 0.5f
        ? 2.0f * x * x
        : 1.0f - pow(-2.0f * x + 2.0f, 2) / 2.0f;
}

void Lab2Application::CleanVAO(GLuint &vao) 
{
    // Your exact same cleanup function
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