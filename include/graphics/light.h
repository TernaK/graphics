#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace graphics {
  struct Light {

    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 ambient;
    GLfloat ph_exp = 32;

    Light(glm::vec3 positon = glm::vec3(20,20,20),
          glm::vec3 color = glm::vec3(0.7),
          glm::vec3 ambient = glm::vec3(0.3))
    : position(positon), color(color), ambient(ambient) {

    }
  };
}
