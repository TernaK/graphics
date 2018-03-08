#pragma once
#include <graphics/buffer_object.h>
#include <graphics/scene_node.h>
#include <graphics/material.h>
#include <numeric>

namespace graphics {
  class Object3D : public SceneNode {
  public:
    Material material;
    
    Object3D() = default;

    Object3D(std::shared_ptr<Geometry> geometry, Material material = Material());

    ~Object3D();

    void set_uniforms(std::shared_ptr<Shader> shader,
                      Transform transform) override;

    std::string get_shader_name() override;
  };
}
