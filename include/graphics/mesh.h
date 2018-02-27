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

  enum struct PrimitiveType {
    box, smooth_sphere, flat_sphere
  };

  class Primitive : public Mesh {
    PrimitiveType type = PrimitiveType::box;
    struct params_t {
      int stacks = 20;
      int slices = 20;
    } params;

    void make_primitive();
    
    void make_box();

    void make_flat_sphere(int stacks = 10, int sections = 10);

    void make_smooth_sphere(int stacks = 10, int sections = 10);

  public:
    Primitive(PrimitiveType type = PrimitiveType::smooth_sphere);

    Primitive(PrimitiveType type, params_t params);

    PrimitiveType get_type();
  };

}
