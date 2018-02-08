#include <graphics/node.h>
using namespace graphics;
using namespace std;

Node::~Node() {
  release_vertex_data();
}

void Node::bind_vertex_data() {
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Node::release_vertex_data() {
  glDeleteBuffers(1, &vbo);
}

Node::Node(const std::vector<GLfloat>& _vertices,
           const std::vector<GLfloat>& _colors)
: vertices(_vertices), colors(_colors) {
  bind_vertex_data();
}


Node::Node(const std::vector<glm::vec3>& _vertices,
           const std::vector<glm::vec4>& _colors) {
//  if(_vertices.size() != _colors.size())
//    throw std::runtime_error("vertices and colors vectors must have the same size");

  for_each(_vertices.begin(), _vertices.end(), [&](const glm::vec3& v) {
    vertices.emplace_back(v.x); vertices.emplace_back(v.y); vertices.emplace_back(v.z);
  });
//  for_each(_colors.begin(), _colors.end(), [&](const glm::vec4& c) {
//    colors.emplace_back(c.a); colors.emplace_back(c.g);
//    colors.emplace_back(c.b); colors.emplace_back(c.a);
//  });
  bind_vertex_data();
}
