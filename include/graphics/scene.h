#pragma once
#include <graphics/canvas.h>
#include <graphics/drawable.h>
#include <graphics/scene_node.h>
#include <graphics/camera.h>
#include <graphics/light.h>
#include <memory>
#include <vector>

namespace graphics {
  // Scene
  //__________________________________________________
  struct Scene {
    std::shared_ptr<SceneNode> root = std::make_shared<SceneNode>();
    std::shared_ptr<Canvas> canvas;
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    std::vector<std::shared_ptr<Light>> lights;
    glm::vec4 clear_color = glm::vec4(0.8,0.8,0.8,1);

    Scene(std::shared_ptr<Canvas> canvas);

    void add_node(std::shared_ptr<SceneNode> node);

    void clear();
  };
}

