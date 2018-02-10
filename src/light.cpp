#include <graphics/light.h>

using namespace std;
using namespace graphics;

PointLight::PointLight(glm::vec3 positon,
                       glm::vec3 color,
                       glm::vec3 ambient)
: position(positon), color(color), ambient(ambient) {

}

void PointLight::set_uniforms(GLuint program) {
  GLint loc = glGetUniformLocation(program, "_light.color");
  glUniform3fv(loc, 1, glm::value_ptr(color));

  loc = glGetUniformLocation(program, "_light.ambient");
  glUniform3fv(loc, 1, glm::value_ptr(ambient));

  loc = glGetUniformLocation(program, "_light.position");
  glUniform3fv(loc, 1, glm::value_ptr(position));

  loc = glGetUniformLocation(program, "_light.ph_exp");
  glUniform1f(loc, ph_exp);
}

void DirectionalLight::set_uniforms(GLuint program) {
  GLint loc = glGetUniformLocation(program, "_light.color");
  glUniform3fv(loc, 1, glm::value_ptr(color));

  loc = glGetUniformLocation(program, "_light.ambient");
  glUniform3fv(loc, 1, glm::value_ptr(ambient));

  loc = glGetUniformLocation(program, "_light.direction");
  glUniform3fv(loc, 1, glm::value_ptr(direction));

  loc = glGetUniformLocation(program, "_light.ph_exp");
  glUniform1f(loc, ph_exp);
}

