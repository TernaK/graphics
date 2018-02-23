#include <graphics/solid.h>
#include <iostream>
#include <exception>
using namespace graphics;
using namespace std;

Solid::~Solid() {
  release_vertex_data();
}

Solid::Solid() {
}

Solid::Solid(const std::vector<glm::vec3>& _vertices,
             std::vector<int> _indices,
             Material _material)
: material(_material) {
  store_vertex_data(_vertices, _indices);
  compute_store_normals();
  bind_vertex_data();
}

Solid::Solid(Geometry& _geometry, Material _material)
: geometry(_geometry), material(_material) {
//  _geometry.get_vertices_and_normals(vertices, normals);
  store_vertex_data();
  bind_vertex_data();
}

void Solid::bind_vertex_data() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  //positions
  positions_vbo = BufferObject<GLfloat, GL_ARRAY_BUFFER>(vertices.data(), vertices.size(), GL_STATIC_DRAW);
  positions_vbo.bind();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  positions_vbo.unbind();
  //normals
  normals_vbo = BufferObject<GLfloat, GL_ARRAY_BUFFER>(normals.data(), normals.size(), GL_STATIC_DRAW);
  normals_vbo.bind();
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(1);
  normals_vbo.unbind();

  glBindVertexArray(0);
}

void Solid::release_vertex_data() {
  normals_vbo.release();
  positions_vbo.release();
}

void Solid::store_vertex_data(const std::vector<glm::vec3>& _vertices,
                              const std::vector<GLint>& _indices) {
  if(_indices.empty()) {
    for(int i = 0; i < _vertices.size(); i++) {
      vertices.push_back(_vertices[i].x);
      vertices.push_back(_vertices[i].y);
      vertices.push_back(_vertices[i].z);
    }
  } else  {
    //unpack vertex data
    for(int i = 0; i < _indices.size(); i++) {
      int idx = _indices[i];
      vertices.push_back(_vertices[idx].x);
      vertices.push_back(_vertices[idx].y);
      vertices.push_back(_vertices[idx].z);
    }
  }
}

void Solid::store_vertex_data() {
  //unpack vertex data
  for(int i = 0; i < geometry.indices.size(); i++) {
    int idx = geometry.indices[i];
    vertices.push_back(geometry.positions[idx].x);
    vertices.push_back(geometry.positions[idx].y);
    vertices.push_back(geometry.positions[idx].z);
    normals.push_back(geometry.normals[idx].x);
    normals.push_back(geometry.normals[idx].y);
    normals.push_back(geometry.normals[idx].z);
  }
}

void Solid::compute_store_normals() {
  for(int i = 0; i < vertices.size() / 9; i++) {
    auto iter = vertices.begin() + i*9;
    glm::vec3 v1(*iter++, *iter++, *iter++);
    glm::vec3 v2(*iter++, *iter++, *iter++);
    glm::vec3 v3(*iter++, *iter++, *iter++);
    glm::vec3 l1 = v1 - v2;
    glm::vec3 l2 = v3 - v2;
    glm::vec3 normal = glm::normalize(glm::cross(l1, l2));
    for(int j = 0; j < 3; j++) {
      normals.push_back(normal.x);
      normals.push_back(normal.y);
      normals.push_back(normal.z);
    }
  }
}

glm::mat4 Solid::get_model_mat() const {
  glm::mat4 model_mat = glm::translate(glm::mat4(1.0), position);
  model_mat = glm::rotate(model_mat, glm::radians(rotation.x), glm::vec3(1,0,0));
  model_mat = glm::rotate(model_mat, glm::radians(rotation.y), glm::vec3(0,1,0));
  model_mat = glm::rotate(model_mat, glm::radians(rotation.z), glm::vec3(0,0,1));
  model_mat = glm::scale(model_mat, scale);
  return model_mat;
}

void Solid::set_uniforms(std::shared_ptr<Shader> shader) const {
  glm::mat4 model_mat = get_model_mat();
  shader->set_uniform("_model_mat", model_mat);
  
  glm::mat3 normal_mat = glm::transpose(glm::inverse(glm::mat3(model_mat)));
  shader->set_uniform("_normal_mat", normal_mat);
}

void Solid::draw(std::shared_ptr<Shader> shader) const {
  material.set_uniforms(shader);
  set_uniforms(shader);

  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size()/3);
  glBindVertexArray(0);
}

ShaderType Solid::get_shader_type() const {
  return ShaderType::Solid;
}
