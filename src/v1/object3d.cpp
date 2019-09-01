#include <graphics/object3d.h>
#include <iostream>
#include <exception>
using namespace graphics;
using namespace std;

// Object3D
//--------------------------------------------------
Object3D::~Object3D() {

}

Object3D::Object3D(std::shared_ptr<Geometry> geometry, Material material)
: SceneNode(geometry), material(material) {
  static const shared_ptr<Shader> default_shader = Shader::make_object3d_shader();
  requires_camera = true;
  shader = default_shader;
}

void Object3D::set_uniforms(Transform transform) {
  material.set_uniforms(shader);
  SceneNode::set_uniforms(transform);
}

// Object3DTex
//--------------------------------------------------
Object3DTex::~Object3DTex() {

}

Object3DTex::Object3DTex(std::shared_ptr<Geometry> geometry,
                         std::shared_ptr<Texture> texture)
: SceneNode(geometry), texture(texture) {
  static const shared_ptr<Shader> default_shader = Shader::make_object3dtex_shader();
  requires_camera = true;
  material.color = VEC3EYE;
  shader = default_shader;
}

void Object3DTex::set_uniforms(Transform transform) {
  texture->activate();
  texture->set_uniforms(shader);
  material.set_uniforms(shader);
  SceneNode::set_uniforms(transform);
}
