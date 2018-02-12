#pragma once
#include <graphics/canvas.h>
#include <graphics/solid.h>
#include <graphics/node.h>
#include <graphics/shader.h>

namespace graphics {
  struct Scene3D {
    std::shared_ptr<Canvas> canvas;
    std::shared_ptr<Light> light = std::make_shared<PointLight>();
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    std::vector<std::shared_ptr<Solid>> solid_nodes;
    std::vector<std::shared_ptr<Node>> nodes;
    std::shared_ptr<SolidShader> solid_shader;
    std::shared_ptr<Node3DShader> node_shader;

    Scene3D(std::shared_ptr<Canvas> canvas);

    void draw_scene();

    void add_drawable(std::shared_ptr<Solid> solid_node);

    void add_drawable(std::shared_ptr<Node> node);

    void clear();
  };

}

