#include <graphics/renderer.h>

using namespace graphics;
using namespace std;
//
//void check_gl_errors() {
//  GLenum err;
//  while((err = glGetError()) && err != GL_NO_ERROR) {
//    switch(err) {
//      case GL_INVALID_ENUM:
//        throw std::runtime_error(std::string("GL_ERROR(GL_INVALID_ENUM)\n"));
//        break;
//      case GL_INVALID_OPERATION:
//        throw std::runtime_error(std::string("GL_ERROR(GL_INVALID_OPERATION)\n"));
//        break;
//      case GL_INVALID_VALUE:
//        throw std::runtime_error(std::string("GL_ERROR(GL_INVALID_VALUE)\n"));
//        break;
//      case GL_INVALID_FRAMEBUFFER_OPERATION:
//        throw std::runtime_error(std::string("GL_ERROR(GL_INVALID_FRAMEBUFFER_OPERATION)\n"));
//        break;
//      default:
//        throw std::runtime_error(std::string("GL_ERROR(" + std::to_string(err) + ")\n"));
//    }
//  }
//}

Renderer::Renderer(std::shared_ptr<Shader> _shader,
                   std::shared_ptr<Camera> _camera,
                   std::shared_ptr<Light> _light)
: shader(_shader), camera(_camera), light(_light) {
  
}

void Renderer::render(const Node& node) {
  shader->use();

  light->set_uniforms(shader->shader_program);
  camera->set_uniforms(shader->shader_program);
  node.set_uniforms(shader->shader_program);

  node.draw();
}

void Renderer::render(const MaterialNode& node) {
  shader->use();

  light->set_uniforms(shader->shader_program);
  camera->set_uniforms(shader->shader_program);
  node.set_uniforms(shader->shader_program);

  node.draw();
}

SpriteRenderer::SpriteRenderer(std::shared_ptr<Shader> _shader)
: shader(_shader) {

}

void SpriteRenderer::render(const graphics::Sprite& sprite) {
  shader->use();
  sprite.draw();
}
