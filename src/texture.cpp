#include <graphics/texture.h>

using namespace std;
using namespace graphics;

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
  cv::Mat temp;
  if(image.channels() == 1)
    cv::cvtColor(image, temp, CV_GRAY2BGR);
  else
    temp = image;

  glGenTextures(1, &texture0);
  glBindTexture(GL_TEXTURE_2D, texture0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp.cols, temp.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, temp.data);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::release_texture_data() {
  glDeleteTextures(1, &texture0);
}

void Texture::activate() {
  glBindTexture(GL_TEXTURE_2D, texture0);
  glActiveTexture(GL_TEXTURE0);
}
