#include "graphics/shader.h"
using namespace graphics;

//--------------------------------------------------Shader - implementation
Shader::Shader(std::string v_text, std::string f_text) {
  int success;
  char infoLog[512];
  //vertex shader
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  const char* vshader_ptr = v_text.c_str();
  glShaderSource(vertex_shader, 1, &vshader_ptr, nullptr);
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
    std::cerr << "vertex shader error: " << infoLog << std::endl;
  }
  //fragment shader
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  const char* fshader_ptr = f_text.c_str();
  glShaderSource(fragment_shader, 1, &fshader_ptr, nullptr);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
    std::cerr << "fragment shader error: " << infoLog << std::endl;
  }
  //program
  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
    std::cerr << "shader program error: " << infoLog << std::endl;
  }
  //cleanup
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

Shader::~Shader() {
  glDeleteProgram(shader_program);
}

void Shader::use() {
  glUseProgram(shader_program);
}

GLint Shader::add_uniform(std::string uniform_name) {
  auto iter = uniforms.find(uniform_name);
  if(iter != uniforms.end())
    return iter->second;
  //if not already found get
  GLint location = glGetUniformLocation(shader_program, uniform_name.c_str());
  if(location == -1)
    throw std::runtime_error(uniform_name + " uniform not found in shader");
  uniforms.insert({uniform_name, location});
  return location;
}

GLint Shader::add_attribute(std::string attribute_name) {
  auto iter = attributes.find(attribute_name);
  if(iter != attributes.end())
    return iter->second;
  //if not already found get
  GLint location = glGetAttribLocation(shader_program, attribute_name.c_str());
  if(location == -1)
    throw std::runtime_error(attribute_name + " attribute not found in shader");
  attributes.insert({attribute_name, location});
  return location;
}

GLint Shader::get_uniform(std::string uniform_name) {
  if(uniforms.find(uniform_name) == uniforms.end())
    throw std::runtime_error(uniform_name + " uniform not found in shader");
  return uniforms[uniform_name];
}

GLint Shader::get_attribute(std::string attribute_name) {
  if(attributes.find(attribute_name) == attributes.end())
    throw std::runtime_error(attribute_name + " attribute not found in shader");
  return attributes[attribute_name];
}

void Shader::set_uniform(std::string uniform, float value) {
  glUniform1f(uniforms.at(uniform), value);
}

void Shader::set_uniform(std::string uniform, int value) {
  glUniform1i(uniforms.at(uniform), value);
}

void Shader::set_uniform(std::string uniform, bool value) {
  glUniform1i(uniforms.at(uniform), value);
}

void Shader::set_uniform(std::string uniform, const glm::vec2& value) {
  glUniform2fv(uniforms.at(uniform), 1, glm::value_ptr(value));
}

void Shader::set_uniform(std::string uniform, const glm::vec3& value) {
  glUniform3fv(uniforms.at(uniform), 1, glm::value_ptr(value));
}

void Shader::set_uniform(std::string uniform, const glm::vec4& value) {
  glUniform4fv(uniforms.at(uniform), 1, glm::value_ptr(value));
}

void Shader::set_uniform(std::string uniform, const glm::mat2& value) {
  glUniformMatrix2fv(uniforms.at(uniform), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_uniform(std::string uniform, const glm::mat3& value) {
  glUniformMatrix3fv(uniforms.at(uniform), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_uniform(std::string uniform, const glm::mat4& value) {
  glUniformMatrix4fv(uniforms.at(uniform), 1, GL_FALSE, glm::value_ptr(value));
}
