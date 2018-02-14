#pragma once
#include <graphics/canvas.h>
#include <graphics/texture.h>
#include <opencv2/opencv.hpp>

namespace graphics {
  class Framebuffer {
    GLuint fbo;
    bool use_color;
    bool use_depth;
    std::shared_ptr<Canvas> canvas;

    void create_textures();
    void create_framebuffer();

  public:
    std::shared_ptr<Texture> color_texture;
    std::shared_ptr<Texture> depth_texture;

    Framebuffer(std::shared_ptr<Canvas> canvas,
                bool use_color = true,
                bool use_depth = false);
    ~Framebuffer();
    void begin_render();
    void end_render();
    cv::Mat read_to_mat();
  };
}
