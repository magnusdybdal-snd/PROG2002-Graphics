#include "Shader.h"

Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc)
{
}

Shader::~Shader()
{
}

void Shader::Bind() const
{
}

void Shader::Unbind() const
{
}

void Shader::UploadUniformFloat2(const std::string &name, const glm::vec2 &vector)
{
}

void Shader::CompileShader(GLenum shaderType, const std::string &shaderSrc)
{
}
