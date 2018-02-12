#pragma once
#include <graphics/shader.h>
#include <graphics/node.h>
#include <graphics/material_node.h>
#include <graphics/camera.h>
#include <graphics/light.h>
#include <graphics/sprite.h>
#include <memory>

namespace graphics {
  struct Renderer {
    std::shared_ptr<MaterialShader> shader;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Light> light;

    Renderer() = default;
    Renderer(std::shared_ptr<MaterialShader> shader,
             std::shared_ptr<Camera> camera,
             std::shared_ptr<Light> light);
    void render(const Node& node);
    void render(const MaterialNode& node);
  };

  struct SpriteRenderer {
    std::shared_ptr<Shader> shader;
    
    SpriteRenderer(std::shared_ptr<Shader> shader);
    void render(const Sprite& sprite);
  };
}
