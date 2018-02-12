#pragma once
#include <graphics/canvas.h>
#include <graphics/light.h>
#include <graphics/camera.h>
#include <graphics/material_node.h>
#include <graphics/node.h>

namespace graphics {
  struct Scene3D {
    std::shared_ptr<Canvas> canvas;
    std::shared_ptr<Light> light = std::make_shared<PointLight>();
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    std::vector<std::shared_ptr<MaterialNode>> material_nodes;
    std::vector<std::shared_ptr<Node>> solid_nodes;

    Scene3D(std::shared_ptr<Canvas> canvas);

    void draw_scene();

    void add_drawable(std::shared_ptr<MaterialNode> material_node);

    void add_drawable(std::shared_ptr<Node> solid_node);

    void clear();
  };

}

