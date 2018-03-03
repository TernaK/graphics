#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <list>
#include <iostream>

namespace graphics { 
  const glm::mat4 MAT4EYE = glm::mat4(1.0);
  const glm::mat3 MAT3EYE = glm::mat3(1.0);
  const glm::vec3 VEC3EYE = glm::vec3(1.0);
  const glm::vec3 VEC3ZERO = glm::vec3(0.0);
  const glm::vec4 VEC4EYE = glm::vec4(1.0);
  constexpr float RAYEPSILON = 1E-4f;

  /// ShapeType
  //--------------------------------------------------
  enum struct ShapeType {
    undefined, plane, box, sphere, flat_sphere
  };

  /// Vertex
  //--------------------------------------------------
  struct Vertex {
    glm::vec3 v;
    glm::vec3 vn;
    glm::vec2 uv;
  };

  /// transform_t
  //--------------------------------------------------
  struct transform_t {
    glm::mat4 model = MAT4EYE;
    glm::mat3 normal = MAT4EYE;
    glm::mat4 model_inv = MAT4EYE;
    glm::mat3 normal_inv = MAT4EYE;
    transform_t operator<<(const transform_t& t) {
      transform_t transform = *this;
      transform.model = t.model * transform.model;
      transform.normal = t.normal * transform.normal;
      transform.model_inv = glm::inverse(transform.model);
      transform.normal_inv = glm::transpose(transform.normal);
      return transform;
    }
  };

  /// Transformable
  //--------------------------------------------------
  struct Transformable {
    glm::vec3 rotation = VEC3ZERO;
    glm::vec3 position = VEC3ZERO; //degrees
    glm::vec3 scale = VEC3EYE;

    transform_t get_transform(glm::mat4 p_model = MAT4EYE);
  };

  /// Facet
  //--------------------------------------------------
  struct Facet {
    int32_t* indices[3];
    int32_t a, b, c;

    Facet(int32_t a, int32_t b, int32_t c);
    int32_t operator[](int idx);
  };

  /// hit_t
  //--------------------------------------------------
  struct hit_t {
    glm::vec3 p;
    glm::vec3 n;
    float dist;
  };

  /// ray_t
  //--------------------------------------------------
  struct ray_t {
    glm::vec3 d;
    glm::vec3 p;
    float l;
  };

  /// primitive_params_t
  //--------------------------------------------------
  struct primitive_params_t {
    int stacks = 20;
    int slices = 20;
    primitive_params_t(int stacks = 20, int slices = 20)
    : stacks(stacks), slices(slices) { };
  };

  /// HitTester
  //--------------------------------------------------
  struct HitTester {
    bool can_test_hit = false;

    virtual bool ray_hit_test(ray_t& ray, hit_t& hit, transform_t& transform);

    bool hit_test_plane(ray_t& ray, hit_t& hit, transform_t& transform,
                        glm::vec3 plane_normal, glm::vec3 offset = VEC3ZERO);

    bool hit_test_box(ray_t& ray, hit_t& hit, transform_t& transform);

    bool hit_test_sphere(ray_t& ray, hit_t& hit, transform_t& transform);
  };

  /// Inheritable
  //--------------------------------------------------
  template<typename T>
  struct Inheritable {
    std::string name = "name";
    std::list<std::shared_ptr<T>> children;

    void add_child(std::shared_ptr<T> child);

    void remove_child(std::shared_ptr<T> child);

    std::vector<std::shared_ptr<T>> get_children_with_name(std::string name);
  };

  template<typename T>
  void Inheritable<T>::add_child(std::shared_ptr<T> child) {
    children.push_back(child);
  }

  template<typename T>
  void Inheritable<T>::remove_child(std::shared_ptr<T> child) {
    children.remove_if([&child](const std::shared_ptr<T>& x) -> bool {
      return child == x;
    });
  }

  template<typename T>
  std::vector<std::shared_ptr<T>> Inheritable<T>::get_children_with_name(std::string name) {
    std::vector<std::shared_ptr<T>> matches;
    for_each(children.begin(), children.end(),
             [&name, &matches](const std::shared_ptr<T>& x) {
               if(x->name == name)
                 matches.push_back(x);
               auto child_matches = x->get_children_with_name(name);
               matches.insert(matches.end(), child_matches.begin(), child_matches.end());
             });
    return matches;
  }

  /// Hittable
  //--------------------------------------------------
  template<typename T>
  struct Hittable {
    bool hit_test_plane(ray_t& ray, hit_t& hit, transform_t& transform,
                        glm::vec3 plane_normal, glm::vec3 offset = VEC3ZERO);

    bool hit_test_box(ray_t& ray, hit_t& hit, transform_t& transform);

    bool hit_test_sphere(ray_t& ray, hit_t& hit, transform_t& transform);
  };

  template<typename T>
  bool Hittable<T>::hit_test_plane(ray_t& ray, hit_t& hit, transform_t& transform,
                                   glm::vec3 plane_normal, glm::vec3 offset) {
    bool did_hit = false;
    glm::vec3 plane_o = glm::vec3(transform.model[3]) + offset;
    glm::vec3 plane_n = glm::normalize(transform.normal * plane_normal);
    float dist;
    if(glm::intersectRayPlane(ray.p, ray.d, plane_o, plane_n, dist)) {
      hit.p = ray.p + dist * ray.d;
      hit.n = plane_n;
      hit.dist = dist;
      glm::vec3 plane_hit = glm::vec3(transform.model_inv * glm::vec4(hit.p - offset, 1.0));
      glm::vec3 diff = glm::abs(plane_hit);
      if( glm::max(diff.x, std::max(diff.y, diff.z) ) < 1 )
        did_hit = true;
    }
    return did_hit;
  }

  template<typename T>
  bool Hittable<T>::hit_test_box(ray_t& ray, hit_t& hit, transform_t& transform) {
    std::vector<std::pair<bool, hit_t>> hit_pairs(6);
    static const std::vector<glm::vec3> normals = {
      glm::vec3(0,1,0), glm::vec3(0,-1,0),
      glm::vec3(-1,0,0), glm::vec3(1,0,0),
      glm::vec3(0,0,1), glm::vec3(0,0,-1),
    };
    for(int i = 0; i < 6; i++) {
      transform_t plane_transform = transform;
      glm::vec3 offset = glm::vec3(plane_transform.model * glm::vec4(normals[i], 1.0));
      hit_pairs[i].first = hit_test_plane(ray, hit_pairs[i].second, plane_transform, normals[i], offset);
    }
    auto end = std::remove_if(hit_pairs.begin(), hit_pairs.end(),
                              [](const std::pair<bool, hit_t>& h) -> bool {
                                return h.first == false;
                              });
    hit_pairs.resize(end - hit_pairs.begin());
    std::sort(hit_pairs.begin(), hit_pairs.end(),
              [](const std::pair<bool, hit_t>& h1, const std::pair<bool, hit_t>& h2) -> bool {
                return h1.second.dist < h2.second.dist;
              });
    if(!hit_pairs.empty()) {
      hit = hit_pairs[0].second;
      return true;
    } else {
      return false;
    }
  }

  template<typename T>
  bool Hittable<T>::hit_test_sphere(ray_t& ray, hit_t& hit, transform_t& transform) {
    bool did_hit = false;
    glm::vec3 p = glm::vec3(transform.model_inv * glm::vec4(ray.p, 1.0));
    glm::vec3 d = glm::vec3(transform.normal_inv * glm::vec4(ray.d, 1.0));
    float a = dot(d,d);
    float b = 2 * dot(p,d);
    float c = dot(p,p) - 1;
    float det = b*b - 4*a*c;
    if(det > 0) {
      float sqrt_det = sqrt(det);
      float t1 = (-b + sqrt_det) / (2*a);
      float t2 = (-b - sqrt_det) / (2*a);
      float t = std::min(t1, t2);
      glm::vec3 hit_sphere = p + t * d;
      if(t > 0 && fabs(glm::length(hit_sphere) - 1) < RAYEPSILON) {
        did_hit = true;
        hit.p = ray.p + t * ray.d;
        hit.n = glm::normalize(transform.normal * hit_sphere);
        hit.dist = glm::length(hit.p - ray.p);
      }
    }
    return did_hit;
  }
}

