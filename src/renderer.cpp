#include <graphics/renderer.h>

using namespace graphics;
using namespace std;

Renderer::Renderer(std::shared_ptr<Shader> _shader,
                   std::shared_ptr<Camera> _camera,
                   std::shared_ptr<Light> _light)
: shader(_shader), camera(_camera), light(_light) {
  
}

void Renderer::set_shader(std::shared_ptr<Shader> _shader) {
  shader = _shader;
}

void Renderer::set_camera(std::shared_ptr<Camera> _camera) {
  camera = _camera;
}

void Renderer::set_light(std::shared_ptr<Light> _light) {
  light = _light;
}

void Renderer::set_camera_uniforms() {
  GLint loc = glGetUniformLocation(shader->shader_program, "_cam_pos");
  glUniform3fv(loc, 1, glm::value_ptr(camera->position));

  glm::mat4 proj_mat = camera->get_proj_mat();
  loc = glGetUniformLocation(shader->shader_program, "_proj_mat");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(proj_mat));

  glm::mat4 view_mat = camera->get_view_mat();
  loc = glGetUniformLocation(shader->shader_program, "_view_mat");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view_mat));
}

void Renderer::set_light_uniforms() {
  GLint loc = glGetUniformLocation(shader->shader_program, "_light.color");
  glUniform3fv(loc, 1, glm::value_ptr(light->color));

  loc = glGetUniformLocation(shader->shader_program, "_light.ambient");
  glUniform3fv(loc, 1, glm::value_ptr(light->ambient));

  loc = glGetUniformLocation(shader->shader_program, "_light.position");
  glUniform3fv(loc, 1, glm::value_ptr(light->position));

  loc = glGetUniformLocation(shader->shader_program, "_light.ph_exp");
  glUniform1f(loc, light->ph_exp);
}

void Renderer::set_node_uniforms(const Node& node) {
  glm::mat4 model_mat = node.get_model_mat();
  GLint loc = glGetUniformLocation(shader->shader_program, "_model_mat");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model_mat));

  glm::mat3 normal_mat = glm::transpose(glm::inverse(glm::mat3(model_mat)));
  loc = glGetUniformLocation(shader->shader_program, "_normal_mat");
  glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(normal_mat));
}

void Renderer::render(const Node& node) {
  shader->use();

  set_light_uniforms();
  set_camera_uniforms();
  set_node_uniforms(node);

  node.draw();
}

void Renderer::set_material_node_uniforms(const MaterialNode& node) {
  glm::mat4 model_mat = node.get_model_mat();
  GLint loc = glGetUniformLocation(shader->shader_program, "_model_mat");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model_mat));

  glm::mat3 normal_mat = glm::transpose(glm::inverse(glm::mat3(model_mat)));
  loc = glGetUniformLocation(shader->shader_program, "_normal_mat");
  glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(normal_mat));

  loc = glGetUniformLocation(shader->shader_program, "_material.color");
  glUniform3fv(loc, 1, glm::value_ptr(node.material.color));

  loc = glGetUniformLocation(shader->shader_program, "_material.alpha");
  glUniform1f(loc, node.material.alpha);
}

void Renderer::render(const MaterialNode& node) {
  shader->use();

  set_light_uniforms();
  set_camera_uniforms();
  set_material_node_uniforms(node);

  node.draw();
}
