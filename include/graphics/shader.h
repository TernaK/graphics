#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include <sstream>
#include <vector>
#include <map>
#include <memory>

namespace graphics {
  const std::string SHADERS_DIR = std::string(GRAPHICS_SHADERS_DIRECTORY);

  struct Shader {
    GLuint fragment_shader;
    GLuint vertex_shader;
    GLuint shader_program;
    std::map<std::string, GLint> uniforms;
    std::map<std::string, GLint> attributes;
    std::string name;

    Shader();
    Shader(std::string vshader_path, std::string fshader_path);
    ~Shader();

    void create_shader_program(std::string vshader_str, std::string fshader_str);
    GLint operator()(std::string uniform_name);
    GLint operator[](std::string attribute_name);
    void use() const;
    GLint add_uniform(std::string uniform_name);
    GLint add_attribute(std::string attribute_name);
    void set_uniform(std::string uniform, float value);
    void set_uniform(std::string uniform, int value);
    void set_uniform(std::string uniform, bool value);
    void set_uniform(std::string uniform, const glm::vec2& value);
    void set_uniform(std::string uniform, const glm::vec3& value);
    void set_uniform(std::string uniform, const glm::vec4& value);
    void set_uniform(std::string uniform, const glm::mat2& value);
    void set_uniform(std::string uniform, const glm::mat3& value);
    void set_uniform(std::string uniform, const glm::mat4& value);

    static std::shared_ptr<Shader> make_simple2d_shader();
    static std::shared_ptr<Shader> make_mesh_point_shader();
    static std::shared_ptr<Shader> make_object3d_point_shader();
    static std::shared_ptr<Shader> make_sprite_shader();
  };
}
