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

    static std::shared_ptr<Geometry> create_box(GLfloat x = 1,
                                                GLfloat y = 1,
                                                GLfloat z = 1);
    
    static std::shared_ptr<Geometry> create_flat_sphere(GLfloat radius = 0.5,
                                                        int stacks = 10,
                                                        int sections = 10);

    static std::shared_ptr<Geometry> create_smooth_sphere(GLfloat radius = 0.5,
                                                        int stacks = 10,
                                                        int sections = 10);

    static std::shared_ptr<Geometry> create_terrain(int z_len, int x_len);
  };
}
