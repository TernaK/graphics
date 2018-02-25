#pragma once
#include <graphics/scene.h>
#include <map>
#include <stack>
#include <set>

namespace graphics {
  struct node_trans_t {
    std::shared_ptr<SceneNode> node;
    transform_t trans;
  };

  struct shader_group_t {
    std::vector<node_trans_t> nodes_trans;
    std::set<std::shared_ptr<Light>> lights;
    std::shared_ptr<Camera> camera;
  };

  class SceneRenderer {
    std::map<std::string, std::shared_ptr<Shader>> shaders;

  public:
    SceneRenderer();

    std::map<std::shared_ptr<Shader>, shader_group_t>
    traverse_scene(std::shared_ptr<Scene> scene);
    
    void traverse_node(std::shared_ptr<SceneNode> node,
                       transform_t parent_transform,
                       std::map<std::shared_ptr<Shader>,shader_group_t>& groups);
    
    void render_scene(std::shared_ptr<Scene> scene);
  };
}
