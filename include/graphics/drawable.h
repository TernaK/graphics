#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace graphics {
  enum struct ShaderType {
    Node3D, Solid, Sprite
  };
  
  class Drawable {
  public:
    ShaderType shader_type = ShaderType::Solid;
    bool hidden = false;
    
    virtual void init_shader_type() = 0;
    virtual void draw(GLuint shader_prog) const = 0;
  };
}
