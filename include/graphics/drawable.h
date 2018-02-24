#pragma once
#include <graphics/shader.h>

namespace graphics {
  enum struct ShaderType {
    Object3D, Mesh, Sprite
  };

  struct Drawable {
    bool hidden = false;

    virtual ShaderType get_shader_type() const = 0;
    virtual void draw(std::shared_ptr<Shader> shader,
                      const glm::mat4& p_model = glm::mat4(1.0),
                      const glm::mat3& p_model_n = glm::mat4(1.0)) const = 0;
  };
}
