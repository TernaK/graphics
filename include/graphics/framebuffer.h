#pragma once
#include <graphics/canvas.h>
#include <graphics/texture.h>

namespace graphics {
  class Framebuffer {
    void create_textures();
    void create_framebuffer();

  public:
    std::shared_ptr<Canvas> canvas;
    std::shared_ptr<Texture> color_texture;
    std::shared_ptr<Texture> depth_texture;
    bool use_color;
    bool use_depth;
    GLuint fbo;

    Framebuffer(std::shared_ptr<Canvas> canvas,
                bool use_color = true,
                bool use_depth = false);
    ~Framebuffer();
    void begin_render();
    void end_render();
  };
}
