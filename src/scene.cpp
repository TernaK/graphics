#include <graphics/scene.h>

using namespace std;
using namespace graphics;

Scene3D::Scene3D(std::shared_ptr<Canvas> _canvas)
: canvas(_canvas) {
  camera->aspect_ratio = GLfloat(_canvas->width)/_canvas->height;
  camera->position = glm::vec3(0,5,10);
  solid_shader = std::make_shared<SolidShader>();
  node_shader = std::make_shared<Node3DShader>();
}

GLuint Scene3D::select_shader(std::shared_ptr<Drawable> drawable) {
  switch(drawable->shader_type) {
    default:
    case ShaderType::Solid:
      solid_shader->use();
      return solid_shader->shader_program;
      
    case ShaderType::Node3D:
      node_shader->use();
      return node_shader->shader_program;
  }
}

void Scene3D::draw_scene() {
  clear();
  for(auto drawable: drawables) {
    GLuint shader_prog = select_shader(drawable);
    light->set_uniforms(shader_prog);
    camera->set_uniforms(shader_prog);
    drawable->draw(shader_prog);
  }
  
  glfwPollEvents();
  glfwSwapBuffers(canvas->window);
  Helper::check_gl_errors();
}

void Scene3D::add_drawable(std::shared_ptr<Drawable> solid_node) {
  drawables.push_back(solid_node);
}

void Scene3D::clear() {
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
