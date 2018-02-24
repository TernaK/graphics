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
                            const glm::mat4& model,
                            const glm::mat3& model_n) const {
  shader->set_uniform("_model_mat", model);
  shader->set_uniform("_normal_mat", model_n);
}

void Object3D::draw(std::shared_ptr<Shader> shader,
                    const glm::mat4& p_model,
                    const glm::mat3& p_model_n,
                    bool draw_children) const {
  if(hidden) return;

  glm::mat4 model = p_model * get_model_mat();
  glm::mat3 model_n = p_model_n * glm::transpose(glm::inverse(glm::mat3(model)));

  if(draw_children) {
    for_each(children.begin(), children.end(), [&](const shared_ptr<Drawable>& x) {
      x->draw(shader, model, model_n);
    });
  }

  if(geometry) {
    material.set_uniforms(shader);
    set_uniforms(shader, model, model_n);
    geometry->draw();
  }
}

ShaderType Object3D::get_shader_type() const {
  return ShaderType::Object3D;
}
