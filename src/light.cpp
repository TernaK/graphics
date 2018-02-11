#include <graphics/light.h>

using namespace std;
using namespace graphics;

void Light::set_uniforms(GLuint program) {
  GLint loc = glGetUniformLocation(program, "_light.color");
  glUniform3fv(loc, 1, glm::value_ptr(color));

  loc = glGetUniformLocation(program, "_light.ambient");
  glUniform3fv(loc, 1, glm::value_ptr(ambient));
}

PointLight::PointLight(glm::vec3 _positon,
                       glm::vec3 _color,
                       glm::vec3 _ambient)
: Light(), position(_positon) {
  color = _color;
  ambient = _ambient;
}

void PointLight::set_uniforms(GLuint program) {
  Light::set_uniforms(program);

  GLint loc = glGetUniformLocation(program, "_light.position");
  glUniform3fv(loc, 1, glm::value_ptr(position));

  loc = glGetUniformLocation(program, "_light.attenuation");
  glUniform3fv(loc, 1, glm::value_ptr(attenuation));
}

DirectionalLight::DirectionalLight(glm::vec3 _direction,
                                   glm::vec3 _color,
                                   glm::vec3 _ambient)
: Light(), direction(_direction) {
  color = _color;
  ambient = _ambient;
}

void DirectionalLight::set_uniforms(GLuint program) {
  Light::set_uniforms(program);

  GLint loc = glGetUniformLocation(program, "_light.direction");
  glUniform3fv(loc, 1, glm::value_ptr(direction));
}

