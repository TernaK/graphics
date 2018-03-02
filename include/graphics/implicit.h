#pragma once
#include <graphics/primitive.h>
#include <graphics/material.h>

namespace graphics {
  struct ImplicitNode : public HitTester, public Transformable {
    std::vector<ShapeType> shapes;
    std::vector<Material> materials;

    ImplicitNode(ShapeType shape, Material material = Material());

    ImplicitNode(std::vector<ShapeType> shape, std::vector<Material> materials);

    bool ray_hit_test(ray_t& ray, hit_t& hit, transform_t& transform) override;
  };
}
