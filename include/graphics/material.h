#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace graphics {
  struct Material_ {
    glm::vec3 color = glm::vec3(0.6, 0.2, 0.7);
    GLfloat shininess = 32;
    float alpha = 1.0;

    Material_() = default;
    Material_(glm::vec3 color, float shininess = 32, float alpha = 1.0)
    : color(color), shininess(shininess), alpha(alpha) {

    }
    void set_uniforms(GLuint program) const {
      GLint loc = glGetUniformLocation(program, "_material.color");
      glUniform3fv(loc, 1, glm::value_ptr(color));

      loc = glGetUniformLocation(program, "_material.alpha");
      glUniform1f(loc, alpha);

      loc = glGetUniformLocation(program, "_material.shininess");
      glUniform1f(loc, shininess);
    }
  };
}
