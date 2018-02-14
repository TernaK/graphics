#pragma once
#include <graphics/canvas.h>
#include <graphics/drawable.h>
#include <graphics/shader.h>
#include <memory>
#include <vector>

namespace graphics {
  class Scene3D {
    GLuint setup_shader(std::shared_ptr<Drawable> drawable);
    void setup_light_camera(GLuint shader_prog);
    
  public:
    std::shared_ptr<Canvas> canvas;
    std::shared_ptr<Light> light = std::make_shared<PointLight>();
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    std::vector<std::shared_ptr<Drawable>> drawables;
    std::shared_ptr<Drawable> background;
    
    std::shared_ptr<SolidShader> solid_shader;
    std::shared_ptr<Node3DShader> node_shader;
    std::shared_ptr<SpriteShader> sprite_shader;

    glm::vec4 clear_color = glm::vec4(0.8,0.8,0.8,1);

    Scene3D(std::shared_ptr<Canvas> canvas);

    void draw_scene();

    void add_drawable(std::shared_ptr<Drawable> solid_node);
    
    void set_background(std::shared_ptr<Drawable> drawable);

    void clear();
  };

}

