#include <graphics/geometry.h>
#include <glm/gtx/string_cast.hpp>
using namespace std;
using namespace graphics;

// Facet
//--------------------------------------------------------------------------------
Facet::Facet(glm::vec3 one, glm::vec3 two, glm::vec3 three) {
  vertices[0] = one;
  vertices[1] = two;
  vertices[2] = three;
}

glm::vec3 Facet::operator[](int idx) {
  return vertices[idx];
}

glm::vec3 Facet::get_normal() {
  return glm::normalize(glm::cross(vertices[2] - vertices[1], vertices[0] - vertices[1]));
}

// Geometry
//--------------------------------------------------------------------------------
Geometry::Geometry(const std::vector<Facet>& _triangles)
: triangles(_triangles) {
  
}

void Geometry::get_vertices_and_normals(std::vector<float>& vertices,
                                        std::vector<float>& normals) {
  vertices.clear();
  normals.clear();
  for(auto& triangle: triangles) {
    auto normal = triangle.get_normal();
    for(int v = 0; v < 3; v++) {
      vertices.push_back(triangle[v].x);
      vertices.push_back(triangle[v].y);
      vertices.push_back(triangle[v].z);
      normals.push_back(normal.x);
      normals.push_back(normal.y);
      normals.push_back(normal.z);
    }
  }
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
  return glm::smoothstep(y0, y1, y);
}

cv::Mat make_perlin_noise(cv::Size size, cv::Mat perlin) {
  cv::Mat result = cv::Mat(size, CV_32F);
  result.forEach<float>([&size, &perlin](float& val, const int* loc) {
    cv::Point2f p(float(loc[1])/size.width * (perlin.cols - 1),
                  float(loc[0])/size.height * (perlin.rows - 1));
    cv::Point l00(floor(p.x), floor(p.y));
    cv::Point l10 = l00 + cv::Point(1,0);
    cv::Point l01 = l00 + cv::Point(0,1);
    cv::Point l11 = l00 + cv::Point(1,1);
    float p00 = perlin.at<float>(l00);
    float p10 = perlin.at<float>(l10);
    float p01 = perlin.at<float>(l01);
    float p11 = perlin.at<float>(l11);
    float x = glm::fract(p.x);
    float y = glm::fract(p.y);
    val = blerp(p00, p10, p01, p11, x, y);
  });
  return result;
}

Geometry Geometry::create_terrain(int z_len, int x_len) {
  vector<vector<glm::vec3>> grid = vector<vector<glm::vec3>>(z_len, vector<glm::vec3>(x_len));
  cv::Mat terrain = cv::Mat(z_len, x_len, CV_32F);
  cv::Mat rand0 = make_random({3,3});
  cv::Mat rand1 = make_random({20,20});
  cv::Mat noise0 = make_perlin_noise(cv::Size(x_len,z_len), rand0);
  cv::Mat noise1 = make_perlin_noise(cv::Size(x_len,z_len), rand1);
  cv::Mat noise = noise0 + 0.03 * noise1;

  for(int z = 0; z < z_len; z++) {
    for(int x = 0; x < x_len; x++) {
      float z_val = (float(z) / (z_len-1)) * 1.0 + -0.5;
      float x_val = (float(x) / (x_len-1)) * 1.0 + -0.5;
      grid[z][x] = glm::vec3(x_val,noise.at<float>(z,x),z_val);
    }
  }

  vector<Facet> temp;
  for(int z = 0; z < grid.size() - 1; z++) {
    for(int x = 0; x < grid.front().size() - 1; x++) {
      temp.emplace_back(grid[z][x], grid[z+1][x], grid[z+1][x+1]);
      temp.emplace_back(grid[z][x], grid[z+1][x+1], grid[z][x+1]);
    }
  }
  return Geometry(temp);
}
