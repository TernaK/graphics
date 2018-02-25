#include <graphics/scene.h>

using namespace std;
using namespace graphics;

// Scene3D
//__________________________________________________
Scene3D::Scene3D(std::shared_ptr<Canvas> _canvas)
: canvas(_canvas) {
  camera->aspect_ratio = canvas->get_aspect_ratio();
  camera->position = glm::vec3(0,5,10);
}

std::shared_ptr<Shader> Scene3D::prepare_shader(std::shared_ptr<Drawable> drawable) {
  switch(drawable->get_shader_type()) {
    default:
    case ShaderType::Object3D:
      object3d_shader->use();
      setup_light_camera(object3d_shader);
      return object3d_shader;

    case ShaderType::Mesh:
      mesh_shader->use();
      setup_light_camera(mesh_shader);
      return mesh_shader;

    case ShaderType::Sprite:
      sprite_shader->use();
      return sprite_shader;
  }
}

void Scene3D::setup_light_camera(std::shared_ptr<Shader> shader) {
  light->set_uniforms(shader);
  camera->set_uniforms(shader);
}

void Scene3D::draw_scene() {
  clear();
  if(background) {
    auto shader = prepare_shader(background);
    background->draw(shader);
    glClear(GL_DEPTH_BUFFER_BIT);
  }

  for(auto drawable: drawables) {
    if(!drawable->hidden) {
      auto shader = prepare_shader(drawable);
      drawable->draw(shader);
    }
  }

  canvas->poll_events();
  canvas->swap_buffers();
  CanvasHelper::check_gl_errors();
}

void Scene3D::add_drawable(std::shared_ptr<Drawable> drawable) {
  drawables.push_back(drawable);
}

void Scene3D::set_background(std::shared_ptr<Drawable> drawable) {
  background = drawable;
}

void Scene3D::clear() {
  glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Scene
//__________________________________________________
Scene::Scene(std::shared_ptr<Canvas> _canvas)
: canvas(_canvas) {
  camera->aspect_ratio = canvas->get_aspect_ratio();
  camera->position = glm::vec3(0,5,10);
}

void Scene::add_node(std::shared_ptr<SceneNode> node) {
  if(!root)
    root = node;
  else
    root->add_child(node);
}

void Scene::clear() {
  glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
