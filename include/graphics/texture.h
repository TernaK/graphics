#pragma once
#include <graphics/shader.h>
#include <opencv2/opencv.hpp>

namespace graphics {
  class Texture {
    GLuint texture_id;
    bool flipped = false; //check this to know how to render

  public:
    cv::Mat image;
    Texture() = default;
    Texture(cv::Mat image);
    Texture(std::string image);
    Texture(GLuint texture_id);
    ~Texture();

    void bind_texture_data();
    void release_texture_data();
    void activate(GLuint texture_unit = 0);
    void set_uniforms(std::shared_ptr<Shader> shader, GLuint texture_unit = 0);
    GLuint get_texture_id();
    void set_filpped();
    bool is_filpped();
  };
}
