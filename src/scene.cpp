#include <graphics/scene.h>

using namespace std;
using namespace graphics;

Scene3D::Scene3D(std::shared_ptr<Canvas> _canvas)
: canvas(_canvas) {
  camera->aspect_ratio = GLfloat(_canvas->width)/_canvas->height;
  camera->position = glm::vec3(0,5,10);
  solid_shader = std::make_shared<SolidShader>();
  node_shader = std::make_shared<Node3DShader>();
  sprite_shader = std::make_shared<SpriteShader>();
}

GLuint Scene3D::setup_shader(std::shared_ptr<Drawable> drawable) {
  switch(drawable->shader_type) {
    default:
    case ShaderType::Solid:
      solid_shader->use();
      setup_light_camera(solid_shader->shader_program);
      return solid_shader->shader_program;
      
    case ShaderType::Node3D:
      node_shader->use();
      setup_light_camera(node_shader->shader_program);
      return node_shader->shader_program;
      
    case ShaderType::Sprite:
      sprite_shader->use();
      return sprite_shader->shader_program;
  }
}

void Scene3D::setup_light_camera(GLuint shader_prog) {
  light->set_uniforms(shader_prog);
  camera->set_uniforms(shader_prog);
}

void Scene3D::draw_scene() {
  clear();
  if(background) {
    GLuint shader_prog = setup_shader(background);
    background->draw(shader_prog);
    glClear(GL_DEPTH_BUFFER_BIT);
  }
  
  for(auto drawable: drawables) {
    if(!drawable->hidden) {
      GLuint shader_prog = setup_shader(drawable);
      drawable->draw(shader_prog);
    }
  }
  
  glfwPollEvents();
  glfwSwapBuffers(canvas->window);
  Helper::check_gl_errors();
}

void Scene3D::add_drawable(std::shared_ptr<Drawable> solid_node) {
  drawables.push_back(solid_node);
}

void Scene3D::set_background(std::shared_ptr<Drawable> drawable) {
  background = drawable;
}

void Scene3D::clear() {
  glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
