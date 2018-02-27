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


//  struct PrimitiveGeometry : public Geometry {
//    PrimitiveGeometry(const std::vector<std::shared_ptr<Mesh>>& meshes);
//    //    bool ray_hit_test(ray_t& ray, hit_t& hit, glm::mat4& p_model);
//  };

  struct PrimitiveSceneNode : public SceneNode {
    PrimitiveSceneNode(std::shared_ptr<PrimitiveGeometry> geometry);
//    std::vector<hit_t> get_ray_hits(ray_t& ray, glm::mat4 p_model = MAT4EYE);
  };

  class RaySceneRenderer : SceneRenderer {
    struct traversed_node_t {
      std::shared_ptr<PrimitiveSceneNode> node;
      glm::mat4 p_model;
      int index;
    };

    struct scene_prim_t {
      int node_idx;
      int mes_idx;
    };

    void add_to_octree(const traversed_node_t& node, Octree<scene_prim_t>& tree);
    
    Octree<scene_prim_t> build_octree(std::shared_ptr<Scene> scene);

    std::vector<traversed_node_t> traverse_scene(std::shared_ptr<Scene> scene);
    
  public:
    cv::Size size;
    RaySceneRenderer();
    void render_scene(std::shared_ptr<Scene> scene) override;
  };
}
