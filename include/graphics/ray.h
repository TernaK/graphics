#pragma once
#include <graphics/implicit.h>
#include <graphics/camera.h>
#include <graphics/light.h>

namespace graphics {
  struct RayScene {
    std::shared_ptr<ImplicitNode> root = std::make_shared<ImplicitNode>();
    
    RayScene() = default;
    
    void add_node(std::shared_ptr<ImplicitNode> node);
  };
  
  class RaySceneRenderer {
    cv::Size frame_size = cv::Size(500,500);
    
    struct traversed_node_t {
      std::shared_ptr<ImplicitNode> node;
      transform_t transform;
    };
    
    void traverse_node(std::shared_ptr<ImplicitNode> node,
                       std::vector<traversed_node_t>& traversed_nodes,
                       transform_t& p_transform);

    std::vector<RaySceneRenderer::traversed_node_t>
    traverse_scene(std::shared_ptr<RayScene> scene);
    
    cv::Mat draw_scene(std::vector<RaySceneRenderer::traversed_node_t>& traversed);

    cv::Vec3f shade_frag(hit_t& hit, Material& material,
                         std::shared_ptr<Camera> camera, std::shared_ptr<Light> light,
                         std::vector<traversed_node_t>& traversed_nodes);

    bool test_shadow(hit_t& hit, std::shared_ptr<Light> light,
                     std::vector<traversed_node_t>& traversed_nodes);

  public:
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    std::shared_ptr<Light> light = std::make_shared<Light>();
    glm::vec4 clear_color = glm::vec4(0.2,0.2,0.2,1);
    int supersample_factor = 1;
    
    RaySceneRenderer() = default;
    
    RaySceneRenderer(cv::Size frame_size, std::shared_ptr<Camera> camera);
    
    cv::Mat render_scene(std::shared_ptr<RayScene> scene);
    
  };
}
