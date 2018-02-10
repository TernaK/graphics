#include <graphics/material_node.h>
#include <iostream>
#include <exception>
using namespace graphics;
using namespace std;

MaterialNode::~MaterialNode() {
  release_vertex_data();
}

MaterialNode::MaterialNode(const std::vector<glm::vec3>& _vertices,
                           std::vector<int> _indices,
                           Material material) {
  if(!_indices.empty())
    store_vertex_data(_vertices, _indices);
  compute_store_normals();
  bind_vertex_data();
}

void MaterialNode::bind_vertex_data() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  //positions
  glGenBuffers(1, &vbo_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  //normals
  glGenBuffers(1, &vbo_normals);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
  glBufferData(GL_ARRAY_BUFFER,
               normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void MaterialNode::release_vertex_data() {
  glDeleteBuffers(1, &vbo_vertices);
  glDeleteBuffers(1, &vbo_normals);
}

void MaterialNode::store_vertex_data(const std::vector<glm::vec3>& _vertices,
                                     const std::vector<GLint>& _indices) {
  //unpack vertex data
  for(int i = 0; i < _indices.size(); i++) {
    int idx = _indices[i];
    vertices.push_back(_vertices[idx].x);
    vertices.push_back(_vertices[idx].y);
    vertices.push_back(_vertices[idx].z);
  }
}

void MaterialNode::compute_store_normals() {
  for(int i = 0; i < vertices.size() / 9; i++) {
    auto iter = vertices.begin() + i*9;
    glm::vec3 v1(*iter++, *iter++, *iter++);
    glm::vec3 v2(*iter++, *iter++, *iter++);
    glm::vec3 v3(*iter++, *iter++, *iter++);
    glm::vec3 l1 = v1 - v2;
    glm::vec3 l2 = v3 - v2;
    glm::vec3 normal = glm::cross(l1, l2);
    for(int j = 0; j < 3; j++) {
      normals.push_back(normal.x);
      normals.push_back(normal.y);
      normals.push_back(normal.z);
    }
  }
}

glm::mat4 MaterialNode::get_model_mat() const {
  glm::mat4 model_mat = glm::translate(glm::mat4(1.0), position);
  model_mat = glm::rotate(model_mat, glm::radians(rotation.x), glm::vec3(1,0,0));
  model_mat = glm::rotate(model_mat, glm::radians(rotation.y), glm::vec3(0,1,0));
  model_mat = glm::rotate(model_mat, glm::radians(rotation.z), glm::vec3(0,0,1));
  model_mat = glm::scale(model_mat, scale);
  return model_mat;
}

void MaterialNode::set_uniforms(GLuint program) const {
  glm::mat4 model_mat = get_model_mat();
  GLint loc = glGetUniformLocation(program, "_model_mat");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model_mat));

  glm::mat3 normal_mat = glm::transpose(glm::inverse(glm::mat3(model_mat)));
  loc = glGetUniformLocation(program, "_normal_mat");
  glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(normal_mat));

  loc = glGetUniformLocation(program, "_material.color");
  glUniform3fv(loc, 1, glm::value_ptr(material.color));

  loc = glGetUniformLocation(program, "_material.alpha");
  glUniform1f(loc, material.alpha);
}

void MaterialNode::draw() const {
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size()/3);
  glBindVertexArray(0);
}
