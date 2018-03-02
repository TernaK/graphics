#pragma once
#include <graphics/implicit.h>

namespace graphics {
  class RaySceneRenderer {
    struct traversed_node_t {
      std::shared_ptr<ImplicitNode> node;
      transform_t transform;
      int index;
    };

    struct scene_prim_t {
      int node_idx;
      int mes_idx;
    };

//    void add_to_octree(const traversed_node_t& node, Octree<scene_prim_t>& tree);
//
//    Octree<scene_prim_t> build_octree(std::shared_ptr<Scene> scene);
//
//    std::vector<traversed_node_t> traverse_scene(std::shared_ptr<Scene> scene);
//
//  public:
//    cv::Size size;
//    RaySceneRenderer();
//    void render_scene(std::shared_ptr<Scene> scene);
  };
}
