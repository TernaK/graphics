#include <graphics/canvas.h>
#include <graphics/shape.h>
#include <graphics/sprite.h>
#include <graphics/camera.h>
#include <graphics/light.h>
using namespace std;
using namespace graphics;

int main(int argc, char* args[]) {
  auto canvas = make_shared<graphics::Canvas>();
  auto solid_shader = Shader::make_solid_point_shader();
  auto sprite_shader = Shader::make_sprite_shader();
  
  auto texture = make_shared<graphics::Texture>("/Users/Terna/Downloads/1387868_720.jpg");
  auto sprite = make_shared<graphics::Sprite>(texture);
  auto solid = SolidShape::make_cube();
  
  PointLight light;
  Camera camera;
  camera.aspect_ratio = canvas->get_aspect_ratio();

  glEnable(GL_DEPTH_TEST);
  while(canvas->still_open()) {
    canvas->clear();
    sprite_shader->use();
    sprite->draw(sprite_shader);
    
    canvas->clear(false, true);
    solid_shader->use();
    solid->rotation.y += 1;
    camera.set_uniforms(solid_shader);
    light.set_uniforms(solid_shader);
    solid->draw(solid_shader);
    
    canvas->poll_events();
    canvas->swap_buffers();
  }
}
