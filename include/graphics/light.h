#pragma once
#include <graphics/shader.h>
#include <memory>

namespace graphics {
  enum struct LightType : int {
    point = 0, directional = 1
  };

  struct Light {
  public:
    LightType type = LightType::point;
    glm::vec3 color = glm::vec3(1.0);
    glm::vec3 position = glm::vec3(2,10,20);
    glm::vec3 attenuation = glm::vec3(1.0, 0.001, 0.0005);
    glm::vec3 direction = glm::vec3(-0.3,-1,-1);

    virtual void set_uniforms(std::shared_ptr<Shader> shader, int idx = 0) const;

    Light(LightType type = LightType::point);
  };
}
