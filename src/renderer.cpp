#include <graphics/renderer.h>

using namespace graphics;
using namespace std;

Renderer::Renderer(std::shared_ptr<Shader> _shader,
                   std::shared_ptr<Camera> _camera,
                   std::shared_ptr<PointLight> _light)
: shader(_shader), camera(_camera), light(_light) {
  
}

void Renderer::set_shader(std::shared_ptr<Shader> _shader) {
  shader = _shader;
}

void Renderer::set_camera(std::shared_ptr<Camera> _camera) {
  camera = _camera;
}

void Renderer::set_light(std::shared_ptr<PointLight> _light) {
  light = _light;
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
