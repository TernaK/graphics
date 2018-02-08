#include <graphics/node.h>
using namespace graphics;
using namespace std;

Node::~Node() {
  release_vertex_data();
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

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Node::release_vertex_data() {
  glDeleteBuffers(1, &vbo_vertices);
  glDeleteBuffers(1, &vbo_colors);
}

void Node::store_vertex_data(const std::vector<glm::vec3>& _vertices,
                             const std::vector<glm::vec4>& _colors) {
  vertices.clear();
  colors.clear();
  for(int i = 0; i < _vertices.size(); i++) {
    vertices.push_back(_vertices[i].x);
    vertices.push_back(_vertices[i].y);
    vertices.push_back(_vertices[i].z);
    colors.push_back(_colors[i].r);
    colors.push_back(_colors[i].g);
    colors.push_back(_colors[i].b);
    colors.push_back(_colors[i].a);
  }
}

Node::Node(const std::vector<GLfloat>& vertices,
           const std::vector<GLfloat>& colors)
: vertices(vertices), colors(colors) {
  bind_vertex_data();
}

Node::Node(const std::vector<glm::vec3>& _vertices,
           const std::vector<glm::vec4>& _colors) {
  if(vertices.size() != colors.size())
    throw std::runtime_error("vertices and colors vectors must have the same size");
  store_vertex_data(_vertices, _colors);
  bind_vertex_data();
}

void Node::render(const Shader& shader) {
  glUseProgram(shader.shader_program);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
//  glDisableVertexAttribArray(0);
//  glDisableVertexAttribArray(1);
//  glBindVertexArray(0);
}
