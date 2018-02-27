#pragma once
#include <graphics/scene_renderer.h>
#include <graphics/octree.h>

namespace graphics {
  struct hit_t {
    glm::vec3 p;
    glm::vec3 n;
    int mesh_idx;
    int index;
  };

  struct ray_t {
    glm::vec3 d;
    glm::vec3 o;
    float l;
  };

  struct PrimitiveGeometry : public Geometry {
    bool hit_test(ray_t ray, hit_t& hit);
  };

  class RaySceneRenderer : SceneRenderer {
  public:
    cv::Size size;
    RaySceneRenderer();
    void render_scene(std::shared_ptr<Scene> scene);
  };
}
