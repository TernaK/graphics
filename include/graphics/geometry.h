#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <functional>
#include <iostream>

namespace graphics {
  struct Facet {
    GLuint* indices[3];
    GLuint a, b, c;
    
    Facet(GLuint a, GLuint b, GLuint c);

    GLuint operator[](int idx);
  };
  
  struct Geometry {
    std::vector<Facet> facets;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tex_coords;
    std::vector<GLuint> indices;

    Geometry() = default;

    Geometry(std::vector<Facet> facets,
             const std::vector<glm::vec3>& positions,
             bool smooth = false,
             std::vector<glm::vec3> normals = {},
             std::vector<glm::vec3> tex_coords = {});

    Geometry(const std::vector<glm::vec3>& positions,
             std::vector<GLuint> indices = {},
             bool smooth = false);

    const Geometry& operator+=(const Geometry& r);

    void compute_smooth_normals();

    void compute_flat_normals();

    void create_indices_from_facets();

    static Geometry create_terrain(int z_len, int x_len);
  };
}
