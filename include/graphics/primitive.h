#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/intersect.hpp>
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

  enum struct ShapeType {
    undefined, plane, box, sphere, flat_sphere
  };

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
    transform_t operator<<(const transform_t& t) {
      transform_t transform = *this;
      transform.model = t.model * transform.model;
      transform.normal = t.normal * transform.normal;
      transform.model_inv = t.model_inv * transform.model_inv;
      transform.normal_inv = t.normal_inv * transform.normal_inv;
      return transform;
    }
  };

  struct Transformable {
    glm::vec3 rotation = VEC3ZERO;
    glm::vec3 position = VEC3ZERO; //degrees
    glm::vec3 scale = VEC3EYE;

    transform_t get_transform(glm::mat4 p_model = MAT4EYE);
  };

  struct Facet {
    int32_t* indices[3];
    int32_t a, b, c;
    
    Facet(int32_t a, int32_t b, int32_t c);
    int32_t operator[](int idx);
  };

  struct hit_t {
    glm::vec3 p;
    glm::vec3 n;
    float dist;
  };

  struct ray_t {
    glm::vec3 d;
    glm::vec3 p;
    float l;
  };

  struct HitTester {
    bool can_test_hit = false;
    
    virtual bool ray_hit_test(ray_t& ray, hit_t& hit, transform_t& transform);
    
    bool hit_test_plane(ray_t& ray, hit_t& hit, transform_t& transform,
                        glm::vec3 plane_normal, glm::vec3 offset = VEC3ZERO);
    
    bool hit_test_box(ray_t& ray, hit_t& hit, transform_t& transform);
    
    bool hit_test_sphere(ray_t& ray, hit_t& hit, transform_t& transform);
  };

  struct primitive_params_t {
    int stacks = 20;
    int slices = 20;
    primitive_params_t(int stacks = 20, int slices = 20)
    : stacks(stacks), slices(slices) { };
  };

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
