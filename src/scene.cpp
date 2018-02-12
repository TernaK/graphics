#include <graphics/scene.h>

using namespace std;
using namespace graphics;

Scene3D::Scene3D(std::shared_ptr<Canvas> _canvas)
: canvas(_canvas) {
    camera->aspect_ratio = GLfloat(_canvas->width)/_canvas->height;
    camera->position = glm::vec3(0,5,8);
}

void Scene3D::draw_scene() {
  clear();
  for(auto m: material_nodes) {
    canvas->material_shader->use();
    light->set_uniforms(canvas->material_shader->shader_program);
    camera->set_uniforms(canvas->material_shader->shader_program);
    m->draw();
  }

  for(auto s: solid_nodes) {
    canvas->solid_shader->use();
    light->set_uniforms(canvas->solid_shader->shader_program);
    camera->set_uniforms(canvas->solid_shader->shader_program);
    s->draw();
  }
  
  glfwPollEvents();
  glfwSwapBuffers(canvas->window);
  Helper::check_gl_errors();
}

void Scene3D::add_drawable(std::shared_ptr<MaterialNode> material_node) {
  material_nodes.push_back(material_node);
  material_node->canvas = canvas;
}

void Scene3D::add_drawable(std::shared_ptr<Node> solid_node) {
  solid_nodes.push_back(solid_node);
  solid_node->canvas = canvas;
}

void Scene3D::clear() {
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
