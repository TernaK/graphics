#include "graphics/graphics.h"
using namespace graphics;

//--------------------------------------------------utility functions
void graphics::check_errors() {
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
