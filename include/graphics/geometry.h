#pragma once
#include <glm/glm.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <functional>
#include <iostream>

namespace graphics {
  struct Facet {
    int indices[3];
    int a, b, c;
    
    Facet(int a, int b, int c);

    int operator[](int idx);
  };
  
  struct Geometry {
    std::vector<Facet> facets;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tex_coords;

    Geometry() = default;

    Geometry(std::vector<Facet> facets,
             const std::vector<glm::vec3>& positions,
             bool smooth = false,
             std::vector<glm::vec3> normals = {},
             std::vector<glm::vec3> tex_coords = {});

    void compute_smooth_normals();

    void compute_flat_normals();

    static Geometry create_terrain(int z_len, int x_len);
  };
}
