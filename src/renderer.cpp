#include <graphics/renderer.h>

using namespace graphics;
using namespace std;

Renderer::Renderer(std::shared_ptr<Shader> _shader,
                   std::shared_ptr<Camera> _camera)
: shader(_shader), camera(_camera) {
  
}

void Renderer::set_shader(std::shared_ptr<Shader> _shader) {
  shader = _shader;
}

void Renderer::set_camera(std::shared_ptr<Camera> _camera) {
  camera = _camera;
}

void Renderer::render(const Node& node) {
  shader->use();
  
  glm::mat4 proj_mat = camera->get_proj_mat();
  GLint loc = glGetUniformLocation(shader->shader_program, "_proj_mat");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(proj_mat));
  
  glm::mat4 view_mat = camera->get_view_mat();
  loc = glGetUniformLocation(shader->shader_program, "_view_mat");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view_mat));
  
  glm::mat4 model_mat = node.get_model_mat();
  loc = glGetUniformLocation(shader->shader_program, "_model_mat");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model_mat));
  
  node.draw();
}
