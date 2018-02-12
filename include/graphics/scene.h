#pragma once
#include <graphics/canvas.h>
//#include <graphics/solid.h>
#include <graphics/node.h>
#include <graphics/shader.h>

namespace graphics {
  class Scene3D {
    GLuint select_shader(std::shared_ptr<Drawable> drawable);
    
  public:
    std::shared_ptr<Canvas> canvas;
    std::shared_ptr<Light> light = std::make_shared<PointLight>();
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    std::vector<std::shared_ptr<Drawable>> drawables;
    std::shared_ptr<SolidShader> solid_shader;
    std::shared_ptr<Node3DShader> node_shader;

    Scene3D(std::shared_ptr<Canvas> canvas);

    void draw_scene();

    void add_drawable(std::shared_ptr<Drawable> solid_node);

    void clear();
  };

}

