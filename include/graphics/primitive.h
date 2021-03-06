#pragma once
#include <graphics/types.h>
#include <glm/gtc/noise.hpp>
#include <opencv2/opencv.hpp>
#include <functional>
#include <exception>

namespace graphics {
  struct PrimitiveMaker {
    static void make_sphere(std::vector<Vertex>& vertices,
                            std::vector<Facet>& facets,
                            int stacks = 10, int slices = 10,
                            bool smooth = true);

    static void make_plane(std::vector<Vertex>& vertices,
                           std::vector<Facet>& facets);

    static void make_box(std::vector<Vertex>& vertices,
                         std::vector<Facet>& facets);

    static void make_terrain(std::vector<Vertex>& vertices,
                             std::vector<Facet>& facets,
                             int z_len, int x_len);

    static void compute_smooth_normals(std::vector<Vertex>& vertices,
                                       std::vector<Facet>& facets);
    
    static glm::vec3 get_facet_normal(const Facet& facet,
                                      const std::vector<Vertex>& vertices);
  };
}
