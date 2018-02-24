#pragma once
#include <graphics/buffer_object.h>
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
  
  struct Mesh {
    std::vector<Facet> facets;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tex_coords;
    std::vector<GLuint> indices;

    GLuint vao;
    BufferObject<GLfloat, GL_ARRAY_BUFFER> normals_vbo;
    BufferObject<GLfloat, GL_ARRAY_BUFFER> positions_vbo;
    BufferObject<GLfloat, GL_ARRAY_BUFFER> texcoords_vbo;
    BufferObject<GLuint, GL_ELEMENT_ARRAY_BUFFER> indices_ebo;

    Mesh() = default;

    Mesh(std::vector<Facet> facets,
             const std::vector<glm::vec3>& positions,
             bool smooth = true,
             std::vector<glm::vec3> normals = {},
             std::vector<glm::vec3> tex_coords = {});

    Mesh(const std::vector<glm::vec3>& positions,
             std::vector<GLuint> indices = {},
             bool smooth = true);

    ~Mesh();

    void bind_vertex_data();

    void compute_smooth_normals();

    void create_indices_from_facets();

    glm::vec3 get_facet_normal(const Facet& facet);

    void draw();
  };
}
