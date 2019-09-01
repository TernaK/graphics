#pragma once
#include "graphics.h"
#include <unordered_map>

namespace graphics {
  //--------------------------------------------------Shader - interface
  /// interface to shaders, their uniforms & attributes
  class Shader {
  public:
    Shader(std::string v_text, std::string f_text);
    ~Shader();
    void use();
    GLint add_uniform(std::string name);
    GLint get_uniform(std::string name);
    GLint add_attribute(std::string name);
    GLint get_attribute(std::string name);
    void set_uniform(std::string uniform, float value);
    void set_uniform(std::string uniform, int value);
    void set_uniform(std::string uniform, bool value);
    void set_uniform(std::string uniform, const glm::vec2& value);
    void set_uniform(std::string uniform, const glm::vec3& value);
    void set_uniform(std::string uniform, const glm::vec4& value);
    void set_uniform(std::string uniform, const glm::mat2& value);
    void set_uniform(std::string uniform, const glm::mat3& value);
    void set_uniform(std::string uniform, const glm::mat4& value);

  private:
    GLuint shader_program;
    std::unordered_map<std::string, GLint> uniforms;
    std::unordered_map<std::string, GLint> attributes;
  };
}
