#include <graphics/object3d.h>
#include <iostream>
#include <exception>
using namespace graphics;
using namespace std;

Object3D::~Object3D() {

}

Object3D::Object3D(std::shared_ptr<Geometry> _geometry, Material _material)
: geometry(_geometry), material(_material) {
  
}

Object3D::Object3D(const std::vector<glm::vec3>& _positions,
             std::vector<GLuint> _indices,
             Material _material)
: material(_material) {
  vector<shared_ptr<Mesh>> meshes = { make_shared<Mesh>(_positions, _indices) };
  geometry = make_shared<Geometry>(meshes);
}

glm::mat4 Object3D::get_model_mat() const {
  glm::mat4 model_mat = glm::translate(glm::mat4(1.0), position);
  model_mat = glm::rotate(model_mat, glm::radians(rotation.x), glm::vec3(1,0,0));
  model_mat = glm::rotate(model_mat, glm::radians(rotation.y), glm::vec3(0,1,0));
  model_mat = glm::rotate(model_mat, glm::radians(rotation.z), glm::vec3(0,0,1));
  model_mat = glm::scale(model_mat, scale);
  return model_mat;
}

void Object3D::set_uniforms(std::shared_ptr<Shader> shader,
                            const glm::mat4& p_model,
                            const glm::mat3& p_model_n) const {
  glm::mat4 model_mat = p_model * get_model_mat();
  shader->set_uniform("_model_mat", model_mat);
  
  glm::mat3 normal_mat = p_model_n * glm::transpose(glm::inverse(glm::mat3(model_mat)));
  shader->set_uniform("_normal_mat", normal_mat);
}

void Object3D::draw(std::shared_ptr<Shader> shader,
                    const glm::mat4& p_model,
                    const glm::mat3& p_model_n) const {
  if(!geometry) return; //for rootnode
  material.set_uniforms(shader);
  set_uniforms(shader, p_model, p_model_n);
  geometry->draw();
}

ShaderType Object3D::get_shader_type() const {
  return ShaderType::Object3D;
}

void Object3D::add_child(std::shared_ptr<Object3D> child) {
  children.push_back(child);
}

void Object3D::remove_child(std::shared_ptr<Object3D> child) {
  children.remove_if([&child](const shared_ptr<Object3D>& x) -> bool {
    return child == x;
  });
}

std::vector<std::shared_ptr<Object3D>> Object3D::get_children_with_name(std::string name) {
  vector<shared_ptr<Object3D>> matches;
  for_each(children.begin(), children.end(),
           [&name, &matches](const shared_ptr<Object3D>& x) {
             if(x->name == name)
               matches.push_back(x);
           });
  return matches;
}
