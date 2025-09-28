#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GLFWApplication
{
    public:
    GLFWApplication(
        const std::string &name, 
        const std::string &version,
        int windowWidth = 1280,
        int windowHeight = 720
    );
    ~GLFWApplication();

    // Initialize GLFW, create window, initialize GLAD
    virtual unsigned Init();

    // Pure virtual function - must be implemented in subclass
    virtual unsigned Run() = 0;
    
    protected:

    GLFWwindow* GetWindow() const { return m_window; }

    // Basic shader compiler helper
    GLuint CompileShader(const std::string& vertexShader, const std::string& fragmentShader);

    private:
    std::string m_name;
    std::string m_version;
    int m_windowWidth = 1280;
    int m_windowHeight = 720;
    std::string m_windowName;
    GLFWwindow* m_window = nullptr;

    // Error callbacks
    static void GLFWErrorCallback(int code, const char* description);
    static void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam);
};