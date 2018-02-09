#pragma once
#include <graphics/shader.h>
#include <graphics/node.h>
#include <graphics/camera.h>
#include <memory>

namespace graphics {
  class Renderer {
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Camera> camera;
    
  public:
    Renderer() = default;
    Renderer(std::shared_ptr<Shader> shader,
             std::shared_ptr<Camera> camera);
    void set_shader(std::shared_ptr<Shader> shader);
    void set_camera(std::shared_ptr<Camera> camera);
    void render(const Node& node);
  };
}
