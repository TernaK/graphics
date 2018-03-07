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
  
  /// primitive_params_t
  //--------------------------------------------------
  struct primitive_params_t {
    int stacks = 20;
    int slices = 20;
    primitive_params_t(int stacks = 20, int slices = 20)
    : stacks(stacks), slices(slices) { };
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
}

