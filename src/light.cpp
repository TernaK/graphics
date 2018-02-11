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

  loc = glGetUniformLocation(program, "_light.shininess");
  glUniform1f(loc, shininess);
}

DirectionalLight::DirectionalLight(glm::vec3 direction,
                                   glm::vec3 color,
                                   glm::vec3 ambient)
: direction(direction), color(color), ambient(ambient) {

}

void DirectionalLight::set_uniforms(GLuint program) {
  GLint loc = glGetUniformLocation(program, "_light.color");
  glUniform3fv(loc, 1, glm::value_ptr(color));

  loc = glGetUniformLocation(program, "_light.ambient");
  glUniform3fv(loc, 1, glm::value_ptr(ambient));

  loc = glGetUniformLocation(program, "_light.direction");
  glUniform3fv(loc, 1, glm::value_ptr(direction));

  loc = glGetUniformLocation(program, "_light.shininess");
  glUniform1f(loc, shininess);
}

