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

Texture::Texture(GLuint _texture_id)
: texture_id(_texture_id){

}

Texture::~Texture() {
  release_texture_data();
}

GLuint Texture::get_texture_id() {
  return texture_id;
}

void Texture::bind_texture_data() {
  cv::Mat temp;
  if(image.channels() == 1)
    cv::cvtColor(image, temp, CV_GRAY2BGRA);
  else if(image.channels() == 3)
    cv::cvtColor(image, temp, CV_BGR2BGRA);

  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp.cols, temp.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, temp.data);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::release_texture_data() {
  glDeleteTextures(1, &texture_id);
}

void Texture::activate(GLuint texture_unit) {
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::set_uniforms(std::shared_ptr<Shader> shader, GLuint texture_unit) {
  std::string texture_name = "_texture" + std::to_string(texture_unit);
  shader->set_uniform(texture_name, (int)texture_unit);
}

void Texture::set_filpped(bool value) {
  flipped = value;
}

bool Texture::is_filpped() {
  return flipped;
}
