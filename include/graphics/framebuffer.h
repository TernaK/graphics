#pragma once
#include <graphics/canvas.h>
#include <graphics/texture.h>
#include <opencv2/opencv.hpp>

namespace graphics {
  /// Renderbuffer
  //--------------------------------------------------
  struct Renderbuffer {
    GLuint rbo;

    Renderbuffer() = default;

    Renderbuffer(GLenum internal_format, std::shared_ptr<Canvas> canvas);

    ~Renderbuffer();

    void bind();

    void unbind();
  };

  struct Depthbuffer : public Renderbuffer {
    Depthbuffer(std::shared_ptr<Canvas> canvas)
    : Renderbuffer(GL_DEPTH24_STENCIL8, canvas) { }
  };

  /// Framebuffer
  //--------------------------------------------------
  class Framebuffer {
    GLuint fbo;
    bool use_color;
    bool use_depth;
    std::shared_ptr<Canvas> canvas;
    std::shared_ptr<Renderbuffer> color_rb;
    std::shared_ptr<Depthbuffer> depth_rb;
    std::vector<GLenum> draw_buffers;

    void create_renderbuffers();
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
