#include <graphics/scene.h>

using namespace std;
using namespace graphics;

Scene3D::Scene3D(std::shared_ptr<Canvas> _canvas)
: canvas(_canvas) {
  camera->aspect_ratio = GLfloat(_canvas->width)/_canvas->height;
  camera->position = glm::vec3(0,5,8);
  solid_shader = std::make_shared<SolidShader>();
  node_shader = std::make_shared<Node3DShader>();
}

void Scene3D::draw_scene() {
  clear();
  for(auto m: solid_nodes) {
    solid_shader->use();
    light->set_uniforms(solid_shader->shader_program);
    camera->set_uniforms(solid_shader->shader_program);
    m->draw(solid_shader->shader_program);
  }

  for(auto s: nodes) {
    node_shader->use();
    light->set_uniforms(node_shader->shader_program);
    camera->set_uniforms(node_shader->shader_program);
    s->draw(node_shader->shader_program);
  }
  
  glfwPollEvents();
  glfwSwapBuffers(canvas->window);
  Helper::check_gl_errors();
}

void Scene3D::add_drawable(std::shared_ptr<Solid> solid_node) {
  solid_nodes.push_back(solid_node);
}

void Scene3D::add_drawable(std::shared_ptr<Node> node) {
  nodes.push_back(node);
}

void Scene3D::clear() {
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
