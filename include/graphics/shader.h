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
  struct Shader {
    GLuint fragment_shader;
    GLuint vertex_shader;
    GLuint shader_program;
    std::map<std::string, GLint> uniforms;

    Shader();
    ~Shader();

    Shader(std::string vshader_path, std::string fshader_path);
    GLint operator()(std::string uniform_name);
    void use() const;
    void unuse() const;
    GLint add_uniform(std::string uniform_name);
    void create_shader_program(std::string vshader_str, std::string fshader_str);
    static const std::string SHADERS_DIR;
  };
}

//  struct LightCameraShader : public Shader {
//    LightCameraShader(std::string v_path = std::string(GRAPHICS_SHADERS_DIRECTORY) + "3d_material_point_vshader.glsl",
//                      std::string f_path = std::string(GRAPHICS_SHADERS_DIRECTORY) + "3d_material_point_fshader.glsl");
//    void set_uniforms(const Light& light, const Camera& camera) const;
//  };
//
//  struct SolidShader : public LightCameraShader {
//    SolidShader(std::string v_path = std::string(GRAPHICS_SHADERS_DIRECTORY) + "3d_material_point_vshader.glsl",
//                   std::string f_path = std::string(GRAPHICS_SHADERS_DIRECTORY) + "3d_material_point_fshader.glsl");
//    void set_uniforms(const Material& material, const Light& light, const Camera& camera) const;
//  };
//
//  struct Node3DShader : public LightCameraShader {
//    Node3DShader(std::string v_path = std::string(GRAPHICS_SHADERS_DIRECTORY) + "3d_solid_point_vshader.glsl",
//                 std::string f_path = std::string(GRAPHICS_SHADERS_DIRECTORY) + "3d_solid_point_fshader.glsl");
//  };
//
//  struct SpriteShader : public Shader {
//    SpriteShader(std::string v_path = std::string(GRAPHICS_SHADERS_DIRECTORY) + "sprite_vshader.glsl",
//                 std::string f_path = std::string(GRAPHICS_SHADERS_DIRECTORY) + "sprite_fshader.glsl");
//  };
//}

