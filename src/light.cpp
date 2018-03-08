#include <graphics/light.h>

using namespace std;
using namespace graphics;

std::string create_uniform_name(int idx, string attribute) {
  return "_lights[" + to_string(idx) + "]." + attribute;
}

Light::Light(LightType type) : type(type) {

}

void Light::set_uniforms(std::shared_ptr<Shader> shader, int idx) const {
  shader->add_uniform(create_uniform_name(idx, "type"));
  shader->add_uniform(create_uniform_name(idx, "color"));
  shader->set_uniform(create_uniform_name(idx, "type"), (int)type);
  shader->set_uniform(create_uniform_name(idx, "color"), color);

  if(type == LightType::point) {
    shader->add_uniform(create_uniform_name(idx, "position"));
    shader->add_uniform(create_uniform_name(idx, "attenuation"));
    shader->set_uniform(create_uniform_name(idx, "position"), position);
    shader->set_uniform(create_uniform_name(idx, "attenuation"), attenuation);
  } else if (type == LightType::directional) {
    shader->add_uniform(create_uniform_name(idx, "direction"));
    shader->set_uniform(create_uniform_name(idx, "direction"), direction);
  }
}
