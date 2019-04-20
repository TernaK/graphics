#include "graphics/point_cloud.h"
#include <glm/gtx/string_cast.hpp>
using namespace std;
using namespace graphics;

// PointCloud
//--------------------------------------------------------------------------------
PointCloud::PointCloud(const std::vector<glm::vec3>& vertices,
                       const std::vector<glm::vec4>& colors)
: vertices(std::move(vertices)), colors(std::move(colors)) {
  init();
}

PointCloud::~PointCloud() {
  vbo.release();
}

void PointCloud::init() {
  pack_data();
  bind_vertex_data();
}

void PointCloud::pack_data() {
  colors_offset =  3 * vertices.size() * sizeof(GLfloat);
  packed_data = std::vector<GLfloat>(vertices.size()*3 + colors.size()*4);
  std::memcpy((char*)packed_data.data(), (char*)vertices.data(), sizeof(GLfloat)*vertices.size()*3);
  std::memcpy(((char*)packed_data.data())+colors_offset, (char*)colors.data(), sizeof(GLfloat)*colors.size()*4);
}

void PointCloud::bind_vertex_data() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  //vertices
  vbo = BufferObject<GLfloat, GL_ARRAY_BUFFER>(packed_data.data(), packed_data.size(), GL_STATIC_DRAW);
  vbo.bind();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (GLvoid*)0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float)*4, (GLvoid*)colors_offset);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  vbo.unbind();

  glBindVertexArray(0);
}

void PointCloud::draw() {
  if(vao == 0) return;
  glBindVertexArray(vao);
  glDrawArrays(GL_POINTS, 0, (GLint)vertices.size());
  glBindVertexArray(0);
}
