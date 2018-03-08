#pragma once
#include <graphics/mesh.h>
#include <glm/gtc/noise.hpp>
#include <memory>

namespace graphics {
  struct Geometry {
    std::vector<std::shared_ptr<Mesh>> meshes;
    bool wire_frame = false;

    Geometry() = default;

    Geometry(const std::vector<std::shared_ptr<Mesh>>& meshes);

    Geometry(ShapeType mesh_type = ShapeType::flat_sphere,
             PrimitiveParams params = PrimitiveParams());

    void draw();
  };
}
