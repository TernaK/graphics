#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <graphics/camera.h>
#include <graphics/light.h>
#include <graphics/material.h>
#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include <sstream>
#include <vector>
#include <map>

namespace graphics {
 const std::string SHADERS_DIR = std::string(GRAPHICS_SHADERS_DIRECTORY);
  struct Shader {
    GLuint fragment_shader;
    GLuint vertex_shader;
    GLuint shader_program;
    std::map<std::string, GLint> uniforms;
    std::map<std::string, GLint> attributes;

    Shader();
    ~Shader();

    Shader(std::string vshader_path, std::string fshader_path);
    GLint operator()(std::string uniform_name);
    GLint operator[](std::string attribute_name);
    void use() const;
    void unuse() const;
    GLint add_uniform(std::string uniform_name);
    GLint add_attribute(std::string attribute_name);
    void create_shader_program(std::string vshader_str, std::string fshader_str);

    static Shader make_simple2d_shader();
    static Shader make_simple3d_shader();
  };
}
