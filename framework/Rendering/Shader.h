#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
  Shader(const std::string &vertexSrc, const std::string &fragmentSrc);
  ~Shader();

  void Bind() const;
  void Unbind() const;
  void UploadUniformFloat2(const std::string& name, const glm::vec2& vector);
  void UploadUniformFloat4(const std::string& name, const glm::vec4& vector);
  void UploadUniformInt2(const std::string& name, const glm::ivec2& vector);
  void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

private:
  GLuint VertexShader;
  GLuint FragmentShader;
  GLuint ShaderProgram;

  void CompileShader(GLenum shaderType, const std::string &shaderSrc);
  GLint GetUniformLocation(const std::string& name);
};

#endif // SHADER_H_