#pragma once
#include <graphics/scene.h>
#include <map>
#include <stack>
#include <set>

namespace graphics {
  struct Renderable {
    std::shared_ptr<SceneNode> node;
    Transform trans;
  };

  struct shader_group_t {
    std::vector<Renderable> renderables;
    std::shared_ptr<Camera> camera;
  };

  class SceneRenderer {
  private:
    std::map<std::shared_ptr<Shader>, shader_group_t>
    traverse_scene(std::shared_ptr<Scene> scene);

    void traverse_node(std::shared_ptr<SceneNode> node,
                       Transform parent_transform,
                       std::map<std::shared_ptr<Shader>,shader_group_t>& groups);

  public:
    SceneRenderer();

    std::map<std::string, std::shared_ptr<Shader>> shaders;
    
    virtual void render_scene(std::shared_ptr<Scene> scene);
  };
}
