#pragma once
#include <graphics/shader.h>

namespace graphics {
  enum struct ShaderType {
    Object3D, Mesh, Sprite
  };

  struct Drawable {
    bool hidden = false;

    virtual ShaderType get_shader_type() const = 0;
    virtual void draw(std::shared_ptr<Shader> shader) const = 0;
  };
}
