#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace graphics {
  struct Light {

    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 ambient;

    Light(glm::vec3 positon = glm::vec3(20,20,20),
          glm::vec3 color = glm::vec3(1),
          glm::vec3 ambient = glm::vec3(0.1))
    : position(positon), color(color), ambient(ambient) {

    }
  };
}
