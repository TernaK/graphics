#pragma once
#include <graphics/primitive.h>
#include <graphics/material.h>
#include <tuple>

namespace graphics {
  struct implicit_test_t {
    bool did_hit;
    hit_t hit;
    Material material;
  };
  
  struct ImplicitNode : public Transformable, public Hittable<ImplicitNode>, public Inheritable<ImplicitNode> {
    std::string name = "node";
    std::vector<ShapeType> shapes;
    std::vector<Material> materials;
    
    ImplicitNode() = default;
    
    ImplicitNode(ShapeType shape, Material material = Material());

    ImplicitNode(std::vector<ShapeType> shape, std::vector<Material> materials);

    implicit_test_t hit_test(ray_t& ray, transform_t& transform);

    bool hit_test_idx(ray_t& ray, hit_t& hit, transform_t& transform, int& idx);
  };
}
