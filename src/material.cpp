#include <graphics/material.h>

using namespace std;
using namespace graphics;

Material::Material(glm::vec3 color, float shininess, float alpha)
: color(color), shininess(shininess), alpha(alpha) {
  
}

void Material::set_uniforms(std::shared_ptr<Shader> shader) const {
  shader->set_uniform("_material.color", color);
  shader->set_uniform("_material.alpha", alpha);
  shader->set_uniform("_material.shininess", shininess);
}
