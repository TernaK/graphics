#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <functional>
#include <iostream>

namespace graphics {
  struct Geometry {
  private:
    struct Triangle {
      glm::vec3 vertices[3];
      
      Triangle(glm::vec3 one, glm::vec3 two, glm::vec3 three);
      glm::vec3 operator[](int idx);
      glm::vec3 get_normal();
    };
    
    static std::vector<std::vector<glm::vec3>>
    create_grid(int z_len, int x_len, std::function<glm::vec3(float,float)> mesh_func);
    
    std::vector<Triangle> triangles;
    
  public:
    Geometry() = default;
    Geometry(const std::vector<Triangle>& triangles);
    void get_vertices_and_normals(std::vector<float>& vertices,
                                  std::vector<float>& normals);
    
    static Geometry create_terrain(int z_len, int x_len);
  };
}
