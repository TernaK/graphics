#include <graphics/texture.h>

using namespace std;
using namespace graphics;

Texture::Texture(cv::Mat _image)
: image(_image) {
  if(image.empty())
    throw std::runtime_error("texture image failed to load");
  bind_texture_data();
}

Texture::Texture(std::string _image)
: image(cv::imread(_image)) {
  if(image.empty())
    throw std::runtime_error("texture image failed to load");
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

  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp.cols, temp.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, temp.data);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::release_texture_data() {
  glDeleteTextures(1, &texture_id);
}

void Texture::activate(GLuint texture_unit) {
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::set_uniforms(GLuint shader_program, GLuint texture_unit) {
  std::string texture_name = "_texture" + std::to_string(texture_unit);
  GLint loc = glGetUniformLocation(shader_program, texture_name.c_str());
  glUniform1i(loc, texture_unit);
}
