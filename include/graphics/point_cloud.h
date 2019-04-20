#pragma once
#include "shader.h"
#include "buffer_object.h"

namespace graphics {
  struct PointCloud {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec4> colors;
    std::vector<GLfloat> packed_data;
    long colors_offset;

    GLuint vao = 0;
    BufferObject<GLfloat, GL_ARRAY_BUFFER> vbo;

    PointCloud() = default;

    PointCloud(const std::vector<glm::vec3>& vertices,
               const std::vector<glm::vec4>& colors);

    ~PointCloud();

    void init();

    void pack_data();

    void bind_vertex_data();

    void draw();
  };
}
