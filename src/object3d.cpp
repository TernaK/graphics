#include <graphics/object3d.h>
#include <iostream>
#include <exception>
using namespace graphics;
using namespace std;

Object3D::~Object3D() {
  release_vertex_data();
}

Object3D::Object3D() {

}

Object3D::Object3D(Geometry& _geometry, Material _material)
: geometry(_geometry), material(_material) {
  bind_vertex_data();
}

Object3D::Object3D(const std::vector<glm::vec3>& _positions,
             std::vector<GLuint> _indices,
             Material _material)
: geometry(Geometry(_positions, _indices)), material(_material) {
  bind_vertex_data();
}

void Object3D::bind_vertex_data() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  //positions
  positions_vbo = BufferObject<GLfloat, GL_ARRAY_BUFFER>(&(geometry.positions.front().x),
                                                         geometry.positions.size() * 3,
                                                         GL_STATIC_DRAW);
  positions_vbo.bind();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  positions_vbo.unbind();
  //normals
  normals_vbo = BufferObject<GLfloat, GL_ARRAY_BUFFER>(&(geometry.normals.front().x),
                                                       geometry.normals.size() * 3,
                                                       GL_STATIC_DRAW);
  normals_vbo.bind();
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(1);
  normals_vbo.unbind();
  //indices
  indices_ebo = BufferObject<GLuint, GL_ELEMENT_ARRAY_BUFFER>(geometry.indices.data(),
                                                              geometry.indices.size(),
                                                              GL_STATIC_DRAW);
  glBindVertexArray(0);
}

void Object3D::release_vertex_data() {
  normals_vbo.release();
  positions_vbo.release();
  indices_ebo.release();
}

glm::mat4 Object3D::get_model_mat() const {
  glm::mat4 model_mat = glm::translate(glm::mat4(1.0), position);
  model_mat = glm::rotate(model_mat, glm::radians(rotation.x), glm::vec3(1,0,0));
  model_mat = glm::rotate(model_mat, glm::radians(rotation.y), glm::vec3(0,1,0));
  model_mat = glm::rotate(model_mat, glm::radians(rotation.z), glm::vec3(0,0,1));
  model_mat = glm::scale(model_mat, scale);
  return model_mat;
}

void Object3D::set_uniforms(std::shared_ptr<Shader> shader) const {
  glm::mat4 model_mat = get_model_mat();
  shader->set_uniform("_model_mat", model_mat);
  
  glm::mat3 normal_mat = glm::transpose(glm::inverse(glm::mat3(model_mat)));
  shader->set_uniform("_normal_mat", normal_mat);
}

void Object3D::draw(std::shared_ptr<Shader> shader) const {
  if(indices_ebo.buffer_object != 0) {
    material.set_uniforms(shader);
    set_uniforms(shader);

    glBindVertexArray(vao);
    indices_ebo.bind();
    glDrawElements(GL_TRIANGLES, geometry.indices.size(), GL_UNSIGNED_INT, 0);
    indices_ebo.unbind();
    glBindVertexArray(0);
  }
}

ShaderType Object3D::get_shader_type() const {
  return ShaderType::Solid;
}
