#pragma once
#include <graphics/mesh.h>

namespace graphics {
  
//  struct transform_t {
//    glm::mat4 model = MAT4EYE;
//    glm::mat3 normal = MAT4EYE;
//    glm::mat4 model_inv = MAT4EYE;
//    glm::mat3 normal_inv = MAT4EYE;
//  };
//
//  struct Transformable {
//    glm::vec3 rotation = glm::vec3(0,0,0);
//    glm::vec3 position = glm::vec3(0,0,0); //degrees
//    glm::vec3 scale = glm::vec3(1,1,1);
//
//    transform_t get_transform(glm::mat4 p_model = MAT4EYE);
//  };
//
//  enum struct PrimitiveType {
//    plane, box, smooth_sphere, flat_sphere
//  };
//
//  struct hit_t {
//    glm::vec3 p;
//    glm::vec3 n;
//  };
//
//  struct ray_t {
//    glm::vec3 d;
//    glm::vec3 p;
//    float l;
//  };
//
//  struct HitTestable {
//    virtual bool ray_hit_test(ray_t& ray, hit_t& hit, transform_t& transform) = 0;
//  };
//
//  struct primitive_params_t {
//    int stacks = 20;
//    int slices = 20;
//    primitive_params_t(int stacks = 20, int slices = 20)
//    : stacks(stacks), slices(slices) {
//
//    };
//  };

//  class Primitive : public Mesh, public HitTestable {
//    PrimitiveType type = PrimitiveType::box;
//    primitive_params_t params;
//
//    void make_primitive();
//
//    void make_plane();
//
//    void make_box();
//
//    void make_flat_sphere(int stacks = 10, int sections = 10);
//
//    void make_smooth_sphere(int stacks = 10, int sections = 10);
//
//  public:
//    Primitive(PrimitiveType type = PrimitiveType::smooth_sphere);
//
//    Primitive(PrimitiveType type, primitive_params_t params);
//
//    PrimitiveType get_type();
//
//    bool ray_hit_test(ray_t& ray, hit_t& hit, transform_t& transform) override;
//  };

}
