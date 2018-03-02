#pragma once
#include <graphics/canvas.h>
#include <graphics/drawable.h>
#include <graphics/scene_node.h>
#include <graphics/camera.h>
#include <graphics/light.h>
#include <memory>
#include <vector>

namespace graphics {
  // Scene3D
  //__________________________________________________
  class Scene3D {
    std::shared_ptr<Shader> prepare_shader(std::shared_ptr<Drawable> drawable);
    void setup_light_camera(std::shared_ptr<Shader> shader);

  public:
    std::shared_ptr<Drawable> root;
    std::shared_ptr<Canvas> canvas;
    std::shared_ptr<Light> light = std::make_shared<Light>();
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    std::vector<std::shared_ptr<Drawable>> drawables;
    std::shared_ptr<Drawable> background;

    std::shared_ptr<Shader> object3d_shader = Shader::make_object3d_shader();
    std::shared_ptr<Shader> mesh_shader = Shader::make_mesh_point_shader();
    std::shared_ptr<Shader> sprite_shader = Shader::make_sprite_shader();

    glm::vec4 clear_color = glm::vec4(0.8,0.8,0.8,1);

    Scene3D(std::shared_ptr<Canvas> canvas);

    void draw_scene();

    void add_drawable(std::shared_ptr<Drawable> solid_node);

    void set_background(std::shared_ptr<Drawable> drawable);

    void clear();
  };

  // Scene
  //__________________________________________________
  struct Scene {
    std::shared_ptr<SceneNode> root = std::make_shared<SceneNode>();
    std::shared_ptr<Canvas> canvas;
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    glm::vec4 clear_color = glm::vec4(0.8,0.8,0.8,1);

    Scene(std::shared_ptr<Canvas> canvas);

    void add_node(std::shared_ptr<SceneNode> node);

    void clear();
  };
}

