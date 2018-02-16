#pragma once
#include <graphics/shader.h>
#include <memory>

namespace graphics {
  struct Material {
    glm::vec3 color = glm::vec3(0.6, 0.2, 0.7);
    GLfloat shininess = 32;
    float alpha = 1.0;

    Material() = default;
    Material(glm::vec3 color, float shininess = 32, float alpha = 1.0);
    void set_uniforms(std::shared_ptr<Shader> shader) const;
  };
}
