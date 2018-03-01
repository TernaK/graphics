#include <graphics/geometry.h>
#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace graphics;

// Geometry
//--------------------------------------------------------------------------------
Geometry::Geometry(const std::vector<std::shared_ptr<Mesh>>& _meshes)
: meshes(std::move(_meshes)) {

}

Geometry::Geometry(const std::vector<glm::vec3>& _positions,
                   const std::vector<GLuint>& _indices) {
  meshes = { make_shared<Mesh>(_positions, _indices) };
}

void Geometry::draw() {
  GLint old_state;
  glGetIntegerv(GL_POLYGON_MODE, &old_state);
  if(wire_frame)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  for(auto mesh: meshes)
    mesh->draw();

  glPolygonMode(GL_FRONT_AND_BACK, old_state);
}

cv::Mat make_random(cv::Size size) {
  cv::Mat mat = cv::Mat(size, CV_32F);
  mat.forEach<float>([](float& val, const int* pos) {
    val = abs( (float(rand() % 512) / 256.0) - 1.0 );
  });
  return mat;
}

float blerp(float p00, float p10, float p01, float p11,
            float x, float y) {
  x = glm::smoothstep(0.0f, 1.0f, x);
  y = glm::smoothstep(0.0f, 1.0f, y);
  float y0 = p00 + x * (p10 - p00);
  float y1 = p01 + x * (p11 - p01);
  return y0 + y * (y1 - y0);
}

cv::Mat make_value_noise(cv::Size size, cv::Mat seed) {
  cv::Mat result = cv::Mat(size, CV_32F);
  result.forEach<float>([&size, &seed](float& val, const int* rc) {
    cv::Point2f loc(float(rc[1])/size.width * (seed.cols - 1),
                    float(rc[0])/size.height * (seed.rows - 1));
    cv::Point l00(floor(loc.x), floor(loc.y));
    cv::Point l10 = l00 + cv::Point(1,0);
    cv::Point l01 = l00 + cv::Point(0,1);
    cv::Point l11 = l00 + cv::Point(1,1);
    float p00 = seed.at<float>(l00);
    float p10 = seed.at<float>(l10);
    float p01 = seed.at<float>(l01);
    float p11 = seed.at<float>(l11);
    float x = glm::fract(loc.x);
    float y = glm::fract(loc.y);
    val = blerp(p00, p10, p01, p11, x, y);
  });
  return result;
}

std::shared_ptr<Geometry> Geometry::create_terrain(int z_len, int x_len) {
  cv::Mat terrain = cv::Mat(z_len, x_len, CV_32F);
  cv::Mat seed0 = make_random({3,3});
  cv::Mat seed1 = make_random({20,20});
  cv::Mat noise0 = make_value_noise(cv::Size(x_len,z_len), seed0);
  cv::Mat noise1 = make_value_noise(cv::Size(x_len,z_len), seed1);
  cv::Mat noise = noise0 + 0.03 * noise1;

  vector<glm::vec3> positions;
  for(int z = 0; z < z_len; z++) {
    for(int x = 0; x < x_len; x++) {
      float z_val = (float(z) / (z_len-1)) * 1.0 + -0.5;
      float x_val = (float(x) / (x_len-1)) * 1.0 + -0.5;
      positions.push_back(glm::vec3(x_val,noise.at<float>(z,x),z_val));
    }
  }

  vector<Facet> facets;
  auto find_idx = [](int z, int x, int X) -> int { return  X * z + x; };
  for(int z = 0; z < z_len - 1; z++) {
    for(int x = 0; x < x_len - 1; x++) {
      int a,b,c;
      //triangle 1
      a = find_idx(z,x, x_len);
      b = find_idx(z+1,x, x_len);
      c = find_idx(z+1,x+1, x_len);
      facets.push_back(Facet(a, b, c));
      //triangle 2
      a = find_idx(z,x, x_len);
      b = find_idx(z+1,x+1, x_len);
      c = find_idx(z,x+1, x_len);
      facets.push_back(Facet(a, b, c));
    }
  }

  vector<shared_ptr<Mesh>> meshes = { make_shared<Mesh>(facets, positions) };
  shared_ptr<graphics::Geometry> terrain_geometry = make_shared<Geometry>(meshes);
  return terrain_geometry;
}

// PrimitiveGeometry
//--------------------------------------------------------------------------------
//PrimitiveGeometry::PrimitiveGeometry(PrimitiveType type)
//: Geometry() {
//  auto mesh = make_shared<Primitive>(type);
//  meshes = { mesh };
//  primitive_types.push_back(type);
//}
//
//PrimitiveGeometry::PrimitiveGeometry(const std::vector<std::shared_ptr<Primitive>>& _meshes)
//: Geometry() {
//  for(auto &m: _meshes) {
//    meshes.push_back(m);
//    primitive_types.push_back(m->get_type());
//  }
//}

