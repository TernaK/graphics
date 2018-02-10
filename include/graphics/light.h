#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace graphics {
  struct Light {
    virtual void set_uniforms(GLuint program) = 0;
  };

  struct PointLight : public Light {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 ambient;
    GLfloat ph_exp = 32;

    PointLight(glm::vec3 positon = glm::vec3(20,20,20),
               glm::vec3 color = glm::vec3(0.7),
               glm::vec3 ambient = glm::vec3(0.3));

    void set_uniforms(GLuint program);
  };

  struct DirectionalLight : public Light {

    glm::vec3 direction;
    glm::vec3 color;
    glm::vec3 ambient;
    GLfloat ph_exp = 32;

    DirectionalLight(glm::vec3 direction = glm::vec3(-1.0),
                     glm::vec3 color = glm::vec3(0.7),
                     glm::vec3 ambient = glm::vec3(0.3));

    void set_uniforms(GLuint program);
  };
}
