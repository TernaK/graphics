#pragma once
#include <graphics/mesh.h>
#include <memory>

namespace graphics {
  struct Geometry {
    std::vector<std::shared_ptr<Mesh>> meshes;
    bool wire_frame = false;

    Geometry() = default;

    Geometry(const std::vector<std::shared_ptr<Mesh>>& meshes);

    Geometry(const std::vector<glm::vec3>& positions,
             const std::vector<GLuint>& indices);

    void draw();

    static std::shared_ptr<Geometry> create_terrain(int z_len, int x_len);
  };

  struct PrimitiveGeometry : public Geometry {
    PrimitiveGeometry(PrimitiveType type = PrimitiveType::smooth_sphere);

    PrimitiveGeometry(const std::vector<std::shared_ptr<Primitive>>& meshes);
  };
}
