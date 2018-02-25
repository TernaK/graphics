#include <graphics/light.h>

using namespace std;
using namespace graphics;

void Light::set_uniforms(std::shared_ptr<Shader> shader, int idx) const {
  auto norm_strength = strength / (strength.x + strength.y + strength.z);
  shader->set_uniform("_light.color", color);
  shader->set_uniform("_light.ambient", ambient);
  shader->set_uniform("_light.strength", norm_strength);
}

PointLight::PointLight(glm::vec3 _positon,
                       glm::vec3 _color,
                       glm::vec3 _ambient)
: Light(), position(_positon) {
  color = _color;
  ambient = _ambient;
}

void PointLight::set_uniforms(std::shared_ptr<Shader> shader, int idx) const {
  Light::set_uniforms(shader);
  shader->set_uniform("_light.position", position);
  shader->set_uniform("_light.attenuation", attenuation);
}

DirectionalLight::DirectionalLight(glm::vec3 _direction,
                                   glm::vec3 _color,
                                   glm::vec3 _ambient)
: Light(), direction(_direction) {
  color = _color;
  ambient = _ambient;
}

void DirectionalLight::set_uniforms(std::shared_ptr<Shader> shader, int idx) const {
  Light::set_uniforms(shader);
  shader->set_uniform("_light.direction", direction);
}

