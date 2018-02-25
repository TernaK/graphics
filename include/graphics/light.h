#pragma once
#include <graphics/shader.h>
#include <memory>

namespace graphics {
  struct Light {
  public:
    glm::vec3 color = glm::vec3(1.0);
    glm::vec3 ambient = glm::vec3(1.0);
    glm::vec3 strength = glm::vec3(1.0,1.0,0.3); //ambient, diffuse, specular
    virtual void set_uniforms(std::shared_ptr<Shader> shader, int idx = 0) const;

  protected:
    Light() = default;
  };

  struct PointLight : public Light {
    glm::vec3 position;
    glm::vec3 attenuation = glm::vec3(1.0, 0.001, 0.0005);

    PointLight(glm::vec3 positon = glm::vec3(2,10,20),
               glm::vec3 color = glm::vec3(1.0),
               glm::vec3 ambient = glm::vec3(1.0));

    void set_uniforms(std::shared_ptr<Shader> shader, int idx = 0) const override;
  };

  struct DirectionalLight : public Light {
    glm::vec3 direction;

    DirectionalLight(glm::vec3 direction = glm::vec3(-0.3,-1,-1),
                     glm::vec3 color = glm::vec3(1.0),
                     glm::vec3 ambient = glm::vec3(0.3));

    void set_uniforms(std::shared_ptr<Shader> shader, int idx = 0) const override;
  };
}
