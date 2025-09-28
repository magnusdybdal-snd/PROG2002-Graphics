#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GLFWApplication
{
    public:
    GLFWApplication(const std::string &name, const std::string &version);
    ~GLFWApplication();

    // Initialisation
    virtual unsigned Init();

    // Run function
    virtual unsigned Run() const = 0;
    
    protected:
    GLFWwindow* GetWindow() const { return m_window; }

    GLuint CompileShader(const std::string& vertexShader, const std::string& fragmentShader);

    private:
    std::string m_name;
    std::string m_version;
    GLFWwindow* m_window = nullptr

    static 
};