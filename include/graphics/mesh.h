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
  
  struct transform_t {
    glm::mat4 model = MAT4EYE;
    glm::mat3 normal = MAT4EYE;
    glm::mat4 model_inv = MAT4EYE;
    glm::mat3 normal_inv = MAT4EYE;
  };
  
  struct Transformable {
    glm::vec3 rotation = glm::vec3(0,0,0);
    glm::vec3 position = glm::vec3(0,0,0); //degrees
    glm::vec3 scale = glm::vec3(1,1,1);
    
    transform_t get_transform(glm::mat4 p_model = MAT4EYE);
  };
  
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
    plane, box, smooth_sphere, flat_sphere
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
    virtual bool ray_hit_test(ray_t& ray, hit_t& hit, transform_t& transform) = 0;
  };
  
  class Primitive : public Mesh, public HitTestable {
    PrimitiveType type = PrimitiveType::box;
    struct params_t {
      int stacks = 20;
      int slices = 20;
    } params;

    void make_primitive();

    void make_plane();
    
    void make_box();

    void make_flat_sphere(int stacks = 10, int sections = 10);

    void make_smooth_sphere(int stacks = 10, int sections = 10);

  public:
    Primitive(PrimitiveType type = PrimitiveType::smooth_sphere);

    Primitive(PrimitiveType type, params_t params);

    PrimitiveType get_type();
    
    bool ray_hit_test(ray_t& ray, hit_t& hit, transform_t& transform) override;
  };

}
