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

  struct Vertex {
    glm::vec3 v;
    glm::vec3 vn;
    glm::vec2 uv;
  };

  struct transform_t {
    glm::mat4 model = MAT4EYE;
    glm::mat3 normal = MAT4EYE;
    glm::mat4 model_inv = MAT4EYE;
    glm::mat3 normal_inv = MAT4EYE;
  };

  struct Transformable {
    glm::vec3 rotation = VEC3ZERO;
    glm::vec3 position = VEC3ZERO; //degrees
    glm::vec3 scale = VEC3EYE;

    transform_t get_transform(glm::mat4 p_model = MAT4EYE);
  };
  
  struct Facet {
    GLuint* indices[3];
    GLuint a, b, c;
    
    Facet(GLuint a, GLuint b, GLuint c);

    GLuint operator[](int idx);
  };

  struct hit_t {
    glm::vec3 p;
    glm::vec3 n;
  };

  struct ray_t {
    glm::vec3 d;
    glm::vec3 p;
    float l;
  };

  struct HitTestable {
    bool can_test_hit = false;
    virtual bool ray_hit_test(ray_t& ray, hit_t& hit, transform_t& transform) {
      return false;
    }
  };

  struct primitive_params_t {
    int stacks = 20;
    int slices = 20;
    primitive_params_t(int stacks = 20, int slices = 20)
    : stacks(stacks), slices(slices) { };
  };

  enum struct MeshType {
    basic, plane, box, sphere, flat_sphere
  };

  struct PrimitiveMaker {
    void make_flat_sphere(std::vector<Vertex>& vertices,
                           std::vector<Facet>& facets,
                           int stacks = 10, int sections = 10);

    void make_sphere(std::vector<Vertex>& vertices,
                     std::vector<Facet>& facets,
                     int stacks = 10, int sections = 10);

    void make_plane(std::vector<Vertex>& vertices, std::vector<Facet>& facets);

    void make_box(std::vector<Vertex>& vertices, std::vector<Facet>& facets);
  };
  
  struct Mesh : public HitTestable, public PrimitiveMaker {
    std::vector<Vertex> vertices;
    std::vector<Facet> facets;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tex_coords;
    std::vector<GLuint> indices;
    MeshType mesh_type = MeshType::basic;

    GLuint vao = 0;
    BufferObject<GLfloat, GL_ARRAY_BUFFER> vertices_vbo;
    BufferObject<GLfloat, GL_ARRAY_BUFFER> positions_vbo;
    BufferObject<GLfloat, GL_ARRAY_BUFFER> normals_vbo;
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

    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<Facet>& facets);

    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<Facet>& facets,
         primitive_params_t params);

    Mesh(MeshType mesh_type, primitive_params_t params);

    ~Mesh();

    void init_from_facets();

    void init_from_positions();

    void init_from_vertices();

    void bind_vertex_data();

    void compute_smooth_normals();

    void create_indices_from_facets();

    glm::vec3 get_facet_normal(const Facet& facet);

    void draw();
  };
}
