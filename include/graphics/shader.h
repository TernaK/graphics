#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include <sstream>

namespace graphics {
  class Shader {
    void create_shader_program(std::string vshader_str, std::string fshader_str);

  public:
    GLuint fragment_shader;
    GLuint vertex_shader;
    GLuint shader_program;
    
    Shader();
    ~Shader();
    Shader(std::string vshader_path, std::string fshader_path);
    void use();
  };
}
