#pragma once
#include <graphics/shader.h>
#include <graphics/node.h>
#include <graphics/material_node.h>
#include <graphics/camera.h>
#include <graphics/light.h>
#include <memory>

namespace graphics {
  class Renderer {
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Light> light;

    void set_light_uniforms();
    
  public:
    Renderer() = default;
    Renderer(std::shared_ptr<Shader> shader,
             std::shared_ptr<Camera> camera,
             std::shared_ptr<Light> light);
    void set_shader(std::shared_ptr<Shader> shader);
    void set_camera(std::shared_ptr<Camera> camera);
    void set_light(std::shared_ptr<PointLight> light);
    void render(const Node& node);
    void render(const MaterialNode& node);
  };
}
