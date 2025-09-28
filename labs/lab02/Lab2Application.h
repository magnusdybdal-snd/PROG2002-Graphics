#ifndef GLFWAPPLICATION_H_
#include "GLFWApplication.h"
#endif

class Lab2Application : public GLFWApplication
{
public:
    Lab2Application(const std::string& name, const std::string& version);
    ~Lab2Application();
    
    // This is where you put your main render loop - just like your current main()
    unsigned Run() override;

private:
    // Helper functions - same as your current ones
    GLuint CreateTriangle();
    GLuint CreateSquare();
    void CleanVAO(GLuint &vao);
    float EaseInOutQuad(float t);
    
    // OpenGL objects
    GLuint m_triangleVAO = 0;
    GLuint m_squareVAO = 0;
    GLuint m_triangleShaderProgram = 0;
    GLuint m_squareShaderProgram = 0;
};