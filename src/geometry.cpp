#include <graphics/geometry.h>
#include <glm/gtx/string_cast.hpp>
using namespace std;
using namespace graphics;

// Facet
//--------------------------------------------------------------------------------
Facet::Facet(int a, int b, int c)
: a(a), b(b), c(c) {
  indices[0] = a;
  indices[1] = b;
  indices[2] = c;
}

int Facet::operator[](int idx) {
  return indices[idx];
}

// Geometry
//--------------------------------------------------------------------------------
Geometry::Geometry(std::vector<Facet> _facets,
                   const std::vector<glm::vec3>& _positions,
                   bool smooth,
                   std::vector<glm::vec3> _normals,
                   std::vector<glm::vec3> _tex_coords) :
facets(std::move(_facets)), positions(std::move(_positions)),
normals(std::move(_normals)), tex_coords(std::move(_tex_coords)) {
  if(_normals.empty()) {
    if(smooth) {
      compute_smooth_normals();
    } else {
      compute_flat_normals();
    }
  }
}

void Geometry::compute_flat_normals() {
  normals = vector<glm::vec3>(positions.size());
  for(const auto& facet: facets) {
    glm::vec3 pa = positions[facet.a];
    glm::vec3 pb = positions[facet.b];
    glm::vec3 pc = positions[facet.c];
    glm::vec3 normal = glm::normalize(glm::cross(pc - pb, pa - pb));
    normals[facet.a] = normal;
    normals[facet.b] = normal;
    normals[facet.c] = normal;
  }
}

void Geometry::compute_smooth_normals() {

}


//void Geometry::get_vertices_and_normals(std::vector<float>& vertices,
//                                        std::vector<float>& normals) {
//  vertices.clear();
//  normals.clear();
//  for(auto& triangle: triangles) {
//    auto normal = triangle.get_normal();
//    for(int v = 0; v < 3; v++) {
//      vertices.push_back(triangle[v].x);
//      vertices.push_back(triangle[v].y);
//      vertices.push_back(triangle[v].z);
//      normals.push_back(normal.x);
//      normals.push_back(normal.y);
//      normals.push_back(normal.z);
//    }
//  }
//}

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

Geometry Geometry::create_terrain(int z_len, int x_len) {
  vector<vector<glm::vec3>> grid = vector<vector<glm::vec3>>(z_len, vector<glm::vec3>(x_len));
  cv::Mat terrain = cv::Mat(z_len, x_len, CV_32F);
  cv::Mat seed0 = make_random({3,3});
  cv::Mat seed1 = make_random({20,20});
  cv::Mat noise0 = make_value_noise(cv::Size(x_len,z_len), seed0);
  cv::Mat noise1 = make_value_noise(cv::Size(x_len,z_len), seed1);
  cv::Mat noise = noise0 + 0.03 * noise1;

  for(int z = 0; z < z_len; z++) {
    for(int x = 0; x < x_len; x++) {
      float z_val = (float(z) / (z_len-1)) * 1.0 + -0.5;
      float x_val = (float(x) / (x_len-1)) * 1.0 + -0.5;
      grid[z][x] = glm::vec3(x_val,noise.at<float>(z,x),z_val);
    }
  }

  vector<Facet> temp;
  vector<glm::vec3> positions;
  auto find_idx = [](int z, int x, int Z, int X) {
    return  
  };
  for(int z = 0; z < grid.size() - 1; z++) {
    for(int x = 0; x < grid.front().size() - 1; x++) {
      //triangle 1
      int a,b,c;
      a =
//      temp.emplace_back(grid[z][x], grid[z+1][x], grid[z+1][x+1]);
//      temp.emplace_back(grid[z][x], grid[z+1][x+1], grid[z][x+1]);
    }
  }
  return Geometry(temp);
}
