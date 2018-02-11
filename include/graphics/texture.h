#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <opencv2/opencv.hpp>
#include <exception>

namespace graphics {
  class Texture {
    GLuint texture0;

  public:
    cv::Mat image;
    Texture() = default;
    Texture(cv::Mat image);
    Texture(std::string image);
    ~Texture();

    void bind_texture_data();
    void release_texture_data();
    void activate();
  };
}
