#pragma once
#include <graphics/geometry.h>

namespace graphics {
  struct hit_t {
    glm::vec3 p;
    glm::vec3 n;
    int mesh_idx;
    int index;
  };

  struct PrimitiveGeometry : public Geometry {
    bool hit_test(glm::vec3 xyz, int idx);
  };

}
