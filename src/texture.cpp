#include <graphics/texture.h>

using namespace std;
using namespace graphics;

void check_gl_errors() {
  GLenum err;
  while((err = glGetError()) && err != GL_NO_ERROR) {
    switch(err) {
      case GL_INVALID_ENUM:
        throw std::runtime_error(std::string("GL_ERROR(GL_INVALID_ENUM)\n"));
        break;
      case GL_INVALID_OPERATION:
        throw std::runtime_error(std::string("GL_ERROR(GL_INVALID_OPERATION)\n"));
        break;
      case GL_INVALID_VALUE:
        throw std::runtime_error(std::string("GL_ERROR(GL_INVALID_VALUE)\n"));
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        throw std::runtime_error(std::string("GL_ERROR(GL_INVALID_FRAMEBUFFER_OPERATION)\n"));
        break;
      default:
        throw std::runtime_error(std::string("GL_ERROR(" + std::to_string(err) + ")\n"));
    }
  }
}

Texture::Texture(cv::Mat image)
: image(image) {
  if(image.empty())
    throw std::runtime_error("texture data is empty");
  bind_texture_data();
}

Texture::Texture(std::string image)
: image(cv::imread(image)) {
  if(image.empty())
    throw std::runtime_error("texture data is empty");
  bind_texture_data();
}

Texture::~Texture() {
  release_texture_data();
}

void Texture::bind_texture_data() {
  glGenTextures(1, &texture0);
  glBindTexture(GL_TEXTURE_2D, texture0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
//  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::release_texture_data() {
  glDeleteTextures(1, &texture0);
}

void Texture::activate() {
  glBindTexture(GL_TEXTURE_2D, texture0);
  glActiveTexture(GL_TEXTURE0);
}
