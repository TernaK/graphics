#include <graphics/material.h>

using namespace std;
using namespace graphics;

Material::Material(glm::vec3 color, float shininess, float alpha)
: color(color), shininess(shininess), alpha(alpha) {
  
}

void Material::set_uniforms(GLuint program) const {
  GLint loc = glGetUniformLocation(program, "_material.color");
  glUniform3fv(loc, 1, glm::value_ptr(color));
  
  loc = glGetUniformLocation(program, "_material.alpha");
  glUniform1f(loc, alpha);
  
  loc = glGetUniformLocation(program, "_material.shininess");
  glUniform1f(loc, shininess);
}
