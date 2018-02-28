#pragma once
#include <graphics/buffer_object.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <functional>
#include <iostream>
#include <exception>

namespace graphics {
  const glm::mat4 MAT4EYE = glm::mat4(1.0);
  const glm::mat3 MAT3EYE = glm::mat3(1.0);
  const glm::vec3 VEC3EYE = glm::vec3(1.0);
  const glm::vec3 VEC3ZERO = glm::vec3(0.0);
  const glm::vec4 VEC4EYE = glm::vec4(1.0);
  constexpr float RAYEPSILON = 0.001;
  
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

    GLuint vao = 0;
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

    void init_from_facets();

    void init_from_positions();

    void bind_vertex_data();

    void compute_smooth_normals();

    void create_indices_from_facets();

    glm::vec3 get_facet_normal(const Facet& facet);

    void draw();
  };
}
