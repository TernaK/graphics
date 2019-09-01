#pragma once
#include "shader.h"
#include "buffer_object.h"
#include "camera.h"
#include "types.h"
#include <numeric>

namespace graphics {
  struct PointCloud : public Transformable {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec4> colors;
    std::vector<GLfloat> packed_data;
    long colors_offset;

    GLuint vao = 0;
    BufferObject<GLfloat, GL_ARRAY_BUFFER> vbo;

    std::shared_ptr<Camera> camera;
    std::shared_ptr<Shader> shader;
    int point_size;

    PointCloud() = default;

    PointCloud(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec4>& colors,
               std::shared_ptr<Camera>& camera, int point_size = 5);

    ~PointCloud();

    void init();

    void pack_data();

    void bind_vertex_data();

    void draw();

    struct PCStats {
      glm::vec3 mean_pt;
      glm::vec3 min_pt;
      glm::vec3 max_pt;
    };

    PCStats get_stats();
  };
}
