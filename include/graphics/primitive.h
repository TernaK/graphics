#pragma once
#include <graphics/types.h>
#include <opencv2/opencv.hpp>
#include <functional>
#include <exception>

namespace graphics {
  struct PrimitiveMaker {
    void make_flat_sphere(std::vector<Vertex>& vertices,
                          std::vector<Facet>& facets,
                          int stacks = 10, int sections = 10);

    void make_sphere(std::vector<Vertex>& vertices,
                     std::vector<Facet>& facets,
                     int stacks = 10, int sections = 10,
                     bool smooth = true);

    void make_plane(std::vector<Vertex>& vertices, std::vector<Facet>& facets);

    void make_box(std::vector<Vertex>& vertices, std::vector<Facet>& facets);

    void make_sphere_grid(std::vector<std::vector<int>> &grid_indices,
                          std::vector<glm::vec3> &positions,
                          int sc, int st);
  };
}
