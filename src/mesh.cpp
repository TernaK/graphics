#include <graphics/mesh.h>
#include <iostream>
#include <exception>
using namespace graphics;
using namespace std;

Mesh::~Mesh() {
  release_vertex_data();
}

Mesh::Mesh() {
//  init_shader_type();
}

Mesh::Mesh(const std::vector<glm::vec3>& _vertices,
           const std::vector<glm::vec4>& _colors,
           std::vector<int> _indices) {
//  init_shader_type();
  if(vertices.size() != colors.size())
    throw std::runtime_error("vertices and colors vectors must have the same size");
  if(!_indices.empty())
    store_vertex_data(_vertices, _colors, _indices);
  compute_store_normals();
  bind_vertex_data();
}

//void Node::init_shader_type() {
//  shader_type = ShaderType::Node3D;
//}

void Mesh::bind_vertex_data() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  //positions
  glGenBuffers(1, &vbo_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  //colors
  glGenBuffers(1, &vbo_colors);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
  glBufferData(GL_ARRAY_BUFFER,
               colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(1);
  //normals
  glGenBuffers(1, &vbo_normals);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
  glBufferData(GL_ARRAY_BUFFER,
               normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Mesh::release_vertex_data() {
  glDeleteBuffers(1, &vbo_vertices);
  glDeleteBuffers(1, &vbo_colors);
  glDeleteBuffers(1, &vbo_normals);
  glDeleteVertexArrays(1, &vao);
}

void Mesh::store_vertex_data(const std::vector<glm::vec3>& _vertices,
                             const std::vector<glm::vec4>& _colors,
                             const std::vector<GLint>& _indices) {
  //unpack vertex data
  for(int i = 0; i < _indices.size(); i++) {
    int idx = _indices[i];
    vertices.push_back(_vertices[idx].x);
    vertices.push_back(_vertices[idx].y);
    vertices.push_back(_vertices[idx].z);

    colors.push_back(_colors[idx].r);
    colors.push_back(_colors[idx].g);
    colors.push_back(_colors[idx].b);
    colors.push_back(_colors[idx].a);
  }
}

void Mesh::compute_store_normals() {
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

glm::mat4 Mesh::get_model_mat() const {
  glm::mat4 model_mat = glm::translate(glm::mat4(1.0), position);
  model_mat = glm::rotate(model_mat, glm::radians(rotation.x), glm::vec3(1,0,0));
  model_mat = glm::rotate(model_mat, glm::radians(rotation.y), glm::vec3(0,1,0));
  model_mat = glm::rotate(model_mat, glm::radians(rotation.z), glm::vec3(0,0,1));
  model_mat = glm::scale(model_mat, scale);
  return model_mat;
}

void Mesh::set_uniforms(std::shared_ptr<Shader> shader) const {
  glm::mat4 model_mat = get_model_mat();
  shader->set_uniform("_model_mat", model_mat);

  //n2.l2 = (R.n1)' . R1.l1 == 0
  //n2.l2 = n1'.R' . R1.l1 --> if R'.R1 == I because R^ = R' (orthogonal, though scaled)
  //hence R' = I.R1^-1 --> R = I.(R^-1)' == (R^-1)'
  //therefore normal rotation R = (R^-1)', transpose of inverse
  glm::mat3 normal_mat = glm::transpose(glm::inverse(glm::mat3(model_mat)));
  shader->set_uniform("_normal_mat", normal_mat);
}

void Mesh::draw(std::shared_ptr<Shader> shader) const {
  set_uniforms(shader);

  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size()/3);
  glBindVertexArray(0);
}

ShaderType Mesh::get_shader_type() const {
  return ShaderType::Mesh;
}
