#pragma once
#include <graphics/mesh.h>
#include <memory>

namespace graphics {
  struct Geometry {
    std::vector<std::shared_ptr<Mesh>> meshes;
    bool wire_frame = false;

    Geometry() = default;

    Geometry(const std::vector<std::shared_ptr<Mesh>>& meshes);

    Geometry(MeshType mesh_type = MeshType::flat_sphere,
             primitive_params_t params = primitive_params_t());

    Geometry(const std::vector<glm::vec3>& positions,
             const std::vector<GLuint>& indices);

    void draw();

    static std::shared_ptr<Geometry> create_terrain(int z_len, int x_len);
  };
}
