#pragma once
#include <graphics/implicit.h>
#include <graphics/camera.h>
#include <graphics/light.h>

namespace graphics {
  struct RayScene {
    std::shared_ptr<ImplicitNode> root;
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    std::shared_ptr<Light> light = std::make_shared<Light>();
    cv::Size frame_size;
    int supersample_factor;
    glm::vec4 clear_color = glm::vec4(0.2,0.2,0.2,1);
    
    RayScene(cv::Size frame_size, std::shared_ptr<Camera> camera);
    
    void add_node(std::shared_ptr<ImplicitNode> node);
  };
  
  class RaySceneRenderer {
    
    struct traversed_node_t {
      std::shared_ptr<ImplicitNode> node;
      transform_t transform;
    };
    
    void traverse_node(std::shared_ptr<ImplicitNode> node, std::vector<traversed_node_t>& traversed_nodes);

    std::vector<traversed_node_t> traverse_scene(std::shared_ptr<RayScene> scene);

  public:
    RaySceneRenderer();
    void render_scene(std::shared_ptr<RayScene> scene);
  };
}
