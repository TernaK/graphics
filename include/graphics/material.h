#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace graphics {
  struct Material {
    glm::vec3 color = glm::vec3(0.6, 0.2, 0.7);
    GLfloat shininess = 32;
    float alpha = 1.0;

    Material() = default;
    Material(glm::vec3 color, float shininess = 32, float alpha = 1.0);
    void set_uniforms(GLuint program) const;
  };
}
