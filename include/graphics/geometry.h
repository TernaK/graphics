#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <functional>
#include <iostream>
#include <exception>

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
             bool smooth = true,
             std::vector<glm::vec3> normals = {},
             std::vector<glm::vec3> tex_coords = {});

    Geometry(const std::vector<glm::vec3>& positions,
             std::vector<GLuint> indices = {},
             bool smooth = true);

    const Geometry& operator+=(const Geometry& r);

    void compute_smooth_normals();

    void create_indices_from_facets();

    glm::vec3 get_facet_normal(const Facet& facet);
    
    static std::shared_ptr<Geometry> create_terrain(int z_len, int x_len);
    static std::shared_ptr<Geometry> create_box(GLfloat x = 0.5,
                                                GLfloat y = 0.5,
                                                GLfloat z = 0.5);
    static std::shared_ptr<Geometry> create_flat_sphere(GLfloat radius = 0.5,
                                                        int stacks = 10,
                                                        int sections = 10);
    static std::shared_ptr<Geometry> create_smooth_sphere(GLfloat radius = 0.5,
                                                        int stacks = 10,
                                                        int sections = 10);
  };
}
