#include <graphics/node.h>
using namespace graphics;
using namespace std;

Node::~Node() {
  release_vertex_data();
}

void Node::bind_vertex_data() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER,
               data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
  //position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  //color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Node::release_vertex_data() {
  glDeleteBuffers(1, &vbo);
}

void Node::pack_vertex_data(const std::vector<GLfloat>& vertices,
                            const std::vector<GLfloat>& colors) {
  data.clear();
  for(int i = 0; i < vertices.size() / 3; i++) {
    data.push_back(vertices[i*3]);
    data.push_back(vertices[i*3+1]);
    data.push_back(vertices[i*3+2]);
    data.push_back(colors[i*4]);
    data.push_back(colors[i*4+1]);
    data.push_back(colors[i*4+2]);
    data.push_back(colors[i*4+3]);
  }
}

void Node::pack_vertex_data(const std::vector<glm::vec3>& vertices,
                            const std::vector<glm::vec4>& colors) {
  data.clear();
  for(int i = 0; i < vertices.size(); i++) {
    data.push_back(vertices[i].x);
    data.push_back(vertices[i].y);
    data.push_back(vertices[i].z);
    data.push_back(colors[i].r);
    data.push_back(colors[i].g);
    data.push_back(colors[i].b);
    data.push_back(colors[i].a);
  }
}

Node::Node(const std::vector<GLfloat>& vertices,
           const std::vector<GLfloat>& colors) {
  pack_vertex_data(vertices, colors);
  bind_vertex_data();
}


Node::Node(const std::vector<glm::vec3>& vertices,
           const std::vector<glm::vec4>& colors) {
//  if(vertices.size() != colors.size())
//    throw std::runtime_error("vertices and colors vectors must have the same size");

  pack_vertex_data(vertices, colors);
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
