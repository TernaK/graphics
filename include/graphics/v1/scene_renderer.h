#pragma once
#include <graphics/scene.h>
#include <stack>
#include <set>

namespace graphics {
  class SceneRenderer {
    struct Renderable {
      std::shared_ptr<SceneNode> node;
      Transform trans;
    };

    struct ShaderGroup {
      std::vector<Renderable> renderables;
      std::shared_ptr<Camera> camera;
    };

    std::map<std::shared_ptr<Shader>,ShaderGroup>
    traverse_scene(std::shared_ptr<Scene> scene);

    void traverse_node(std::shared_ptr<SceneNode> node,
                       Transform parent_transform,
                       std::map<std::shared_ptr<Shader>,ShaderGroup>& groups);

  public:
    SceneRenderer() = default;
    
    void render_scene(std::shared_ptr<Scene> scene);
  };
}
