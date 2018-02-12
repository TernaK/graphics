#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace graphics {
  struct Light {
  public:
    glm::vec3 color = glm::vec3(0.7);
    glm::vec3 ambient = glm::vec3(0.3);
    virtual void set_uniforms(GLuint program) const ;

  protected:
    Light() = default;
  };

  struct PointLight : public Light {
    glm::vec3 position;
    glm::vec3 attenuation = glm::vec3(1.0, 0.001, 0.0005);

    PointLight(glm::vec3 positon = glm::vec3(2,10,20),
               glm::vec3 color = glm::vec3(0.7),
               glm::vec3 ambient = glm::vec3(0.3));

    void set_uniforms(GLuint program) const ;
  };

  struct DirectionalLight : public Light {
    glm::vec3 direction;

    DirectionalLight(glm::vec3 direction = glm::vec3(-0.3,-1,-1),
                     glm::vec3 color = glm::vec3(0.7),
                     glm::vec3 ambient = glm::vec3(0.3));

    void set_uniforms(GLuint program) const ;
  };
}
