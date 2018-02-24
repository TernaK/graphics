#pragma once
#include <graphics/mesh.h>
#include <memory>

namespace graphics {
  struct Geometry {
    std::vector<std::shared_ptr<Mesh>> meshes;

    Geometry() = default;

    Geometry(const std::vector<std::shared_ptr<Mesh>>& meshes);

    void draw();

    static std::shared_ptr<Geometry> create_box(GLfloat x = 0.5,
                                                GLfloat y = 0.5,
                                                GLfloat z = 0.5);
    
    static std::shared_ptr<Geometry> create_flat_sphere(GLfloat radius = 0.5,
                                                        int stacks = 10,
                                                        int sections = 10);

    static std::shared_ptr<Geometry> create_smooth_sphere(GLfloat radius = 0.5,
                                                        int stacks = 10,
                                                        int sections = 10);

    static std::shared_ptr<Geometry> create_terrain(int z_len, int x_len);
  };
}
