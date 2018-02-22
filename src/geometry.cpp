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
std::vector<std::vector<glm::vec3>>
Geometry::create_grid(int z_len, int x_len, std::function<glm::vec3(float,float)> grid_func) {
  vector<vector<glm::vec3>> grid = vector<vector<glm::vec3>>(z_len, vector<glm::vec3>(x_len));
  for(int z = 0; z < z_len; z++) {
    for(int x = 0; x < x_len; x++) {
      float z_val = (float(z) / (z_len-1)) * 1.0 + -0.5;
      float x_val = (float(x) / (x_len-1)) * 1.0 + -0.5;
      grid[z][x] = grid_func(z_val,x_val);
    }
  }
  return grid;
}

Geometry Geometry::create_terrain(int z_len, int x_len) {
  auto terrain_func = [](float z, float x) -> glm::vec3 {
    return glm::vec3(x, 0.5 + 0.1 * (fmod(rand(), 100.0)/100.0),z);//glm::vec3(x, 0.5 + 0.03 * (sin(2.0*M_PI*x/0.5) + sin(2.0*M_PI*z/0.5)), z);
  };

  auto grid = create_grid(z_len, x_len, terrain_func);
  vector<Facet> temp;
  for(int z = 0; z < grid.size() - 1; z++) {
    for(int x = 0; x < grid.front().size() - 1; x++) {
      temp.emplace_back(grid[z][x], grid[z+1][x], grid[z+1][x+1]);
      temp.emplace_back(grid[z][x], grid[z+1][x+1], grid[z][x+1]);
    }
  }
  return Geometry(temp);
}
