#include <graphics/object3d.h>
#include <iostream>
#include <exception>
using namespace graphics;
using namespace std;

Object3D::~Object3D() {

}

Object3D::Object3D(std::shared_ptr<Geometry> geometry, Material material)
: SceneNode(geometry), material(material) {
  requires_camera = true;
}

void Object3D::set_uniforms(std::shared_ptr<Shader> shader,
                            Transform transform) {
  material.set_uniforms(shader);
  SceneNode::set_uniforms(shader, transform);
}

std::string Object3D::get_shader_name() {
  return "object3d";
}
