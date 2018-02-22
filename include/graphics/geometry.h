#pragma once
#include <glm/glm.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <functional>
#include <iostream>

namespace graphics {
  /// Hold 3 vertices of a triangle facet (counter clockwise vertex order)
  struct Facet {
    glm::vec3 vertices[3];
    
    Facet(glm::vec3 one, glm::vec3 two, glm::vec3 three);
    
    /// Get a facet vertex at an index
    glm::vec3 operator[](int idx);
    
    /// Get a normal for the facet
    glm::vec3 get_normal();
  };
  
  struct Geometry {
  private:
    std::vector<Facet> triangles;
    
    /// Create 2D grid in the z-x plane (coordinates are [z,x] format)
    static std::vector<std::vector<glm::vec3>>
    create_grid(int z_len, int x_len, std::function<glm::vec3(float,float)> grid_func);
    
  public:
    Geometry() = default;
    
    /// Hold a vector of Facets (counter clockwise vertex order)
    Geometry(const std::vector<Facet>& facet);
    
    /// Get raw float arrays for vertices and normals
    void get_vertices_and_normals(std::vector<float>& vertices,
                                  std::vector<float>& normals);
    
    static Geometry create_terrain(int z_len, int x_len);
  };
}
