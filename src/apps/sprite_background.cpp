#include <graphics/canvas.h>
#include <graphics/sprite.h>
using namespace std;
using namespace graphics;

int main(int argc, char* args[]) {
  auto canvas = make_shared<graphics::Canvas>();
  auto sprite_shader = Shader::make_sprite_shader();

  auto texture = make_shared<graphics::Texture>(graphics::TEXTURES_DIR + "/triangles.jpg");
  auto sprite = make_shared<graphics::Sprite>(texture);

  glEnable(GL_DEPTH_TEST);
  while(canvas->still_open()) {
    canvas->clear();

    sprite_shader->use();
    sprite->draw(sprite_shader);

    canvas->poll_events();
    canvas->swap_buffers();
  }
}
