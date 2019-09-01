#include "graphics/texture.h"
using namespace graphics;

Texture::Texture(unsigned char* data, int width, int height, bool bgr) {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  auto pixel_format = bgr ? GL_BGR : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
               pixel_format, GL_UNSIGNED_BYTE, (void*)data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::activate() {
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(GL_TEXTURE_2D, texture);
}
