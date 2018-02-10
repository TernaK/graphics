#include <graphics/node.h>
#include <iostream>
#include <exception>
using namespace graphics;
using namespace std;

Node::~Node() {
  release_vertex_data();
}

Node::Node(const std::vector<GLfloat>& _vertices,
           const std::vector<GLfloat>& _colors)
: vertices(_vertices), colors(_colors) {
  bind_vertex_data();
}

Node::Node(const std::vector<glm::vec3>& _vertices,
           const std::vector<glm::vec4>& _colors,
           std::vector<int> _indices) {
  if(vertices.size() != colors.size())
    throw std::runtime_error("vertices and colors vectors must have the same size");
  if(!_indices.empty())
    store_vertex_data(_vertices, _colors, _indices);
  bind_vertex_data();
}

void Node::bind_vertex_data() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  //position
  glGenBuffers(1, &vbo_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  //color
  glGenBuffers(1, &vbo_colors);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
  glBufferData(GL_ARRAY_BUFFER,
               colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(1);
//  //element array
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Node::release_vertex_data() {
  glDeleteBuffers(1, &vbo_vertices);
  glDeleteBuffers(1, &vbo_colors);
//  glDeleteBuffers(1, &ebo);
}

void Node::store_vertex_data(const std::vector<glm::vec3>& _vertices,
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

glm::mat4 Node::get_model_mat() const {
  glm::mat4 model_mat = glm::translate(glm::mat4(1.0), position);
  model_mat = glm::rotate(model_mat, glm::radians(rotation.x), glm::vec3(1,0,0));
  model_mat = glm::rotate(model_mat, glm::radians(rotation.y), glm::vec3(0,1,0));
  model_mat = glm::rotate(model_mat, glm::radians(rotation.z), glm::vec3(0,0,1));
  model_mat = glm::scale(model_mat, scale);
  return model_mat;
}

void Node::draw() const {
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
  glBindVertexArray(0);
}
