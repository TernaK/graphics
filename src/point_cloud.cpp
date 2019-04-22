#include "graphics/point_cloud.h"
#include <glm/gtx/string_cast.hpp>
using namespace std;
using namespace graphics;

// PointCloud
//--------------------------------------------------------------------------------
PointCloud::PointCloud(const std::vector<glm::vec3>& vertices,
                       const std::vector<glm::vec4>& colors,
                       std::shared_ptr<Camera>& camera,
                       int point_size)
: vertices(std::move(vertices)), colors(std::move(colors)), camera(camera), point_size(point_size) {
  init();

  const static std::shared_ptr<Shader> default_shader = Shader::make_point_cloud_shader();
  shader = default_shader;
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

  shader->use();
  shader->set_uniform("_proj_mat", camera->get_proj_mat());
  shader->set_uniform("_view_mat", camera->get_view_mat());
  shader->set_uniform("_model_mat", get_transform().model);

  glPointSize(point_size);
  glBindVertexArray(vao);
  glDrawArrays(GL_POINTS, 0, (GLint)vertices.size());
  glBindVertexArray(0);
}

PointCloud::PCStats PointCloud::get_stats() {
  //sort each one and take 25th 50th & 75th percentiles as min mean and max
  std::vector<int> sort_idxs(vertices.size());
  std::iota(sort_idxs.begin(), sort_idxs.end(), 0);
  auto x_idxs = std::move(sort_idxs);
  auto y_idxs = x_idxs;
  auto z_idxs = x_idxs;
  std::sort(x_idxs.begin(), x_idxs.end(), [&](const int &a, const int &b) -> bool {
    return vertices[a].x < vertices[b].x;
  });
  std::sort(y_idxs.begin(), y_idxs.end(), [&](const int &a, const int &b) -> bool {
    return vertices[a].y < vertices[b].y;
  });
  std::sort(z_idxs.begin(), z_idxs.end(), [&](const int &a, const int &b) -> bool {
    return vertices[a].z < vertices[b].z;
  });

  int idx_minth = round(vertices.size() * 0.2);
  int idx_meanth = round(vertices.size() * 0.5);
  int idx_maxth = round(vertices.size() * 0.8);

  PCStats stats;
  stats.mean_pt = glm::vec3(vertices[x_idxs[idx_meanth]].x, vertices[y_idxs[idx_meanth]].y, vertices[z_idxs[idx_meanth]].z);
  stats.min_pt = glm::vec3(vertices[x_idxs[idx_minth]].x, vertices[y_idxs[idx_minth]].y, vertices[z_idxs[idx_minth]].z);
  stats.max_pt = glm::vec3(vertices[x_idxs[idx_maxth]].x, vertices[y_idxs[idx_maxth]].y, vertices[z_idxs[idx_maxth]].z);
  return stats;
}
