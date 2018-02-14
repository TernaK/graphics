#include <graphics/framebuffer.h>
#include <graphics/scene.h>
#include <graphics/shape.h>
#include <graphics/sprite.h>
using namespace std;

int main() {
  shared_ptr<graphics::Canvas> canvas = make_shared<graphics::Canvas>();
  graphics::Scene3D scene(canvas);
  scene.sprite_shader = make_shared<graphics::SpriteShader>(std::string(GRAPHICS_SHADERS_DIRECTORY) + "sprite_vshader.glsl",
                                                            std::string(GRAPHICS_SHADERS_DIRECTORY) + "sprite_fshader_post.glsl");
  graphics::Framebuffer framebuffer(canvas, true, true);

  shared_ptr<graphics::Solid> solid = graphics::SolidShape::make_sphere(2);
  shared_ptr<graphics::Sprite> sprite = std::make_shared<graphics::Sprite>(framebuffer.color_texture);

  while(!glfwWindowShouldClose(canvas->window)) {
    //render the scene
    scene.drawables.clear();
    framebuffer.begin_render();
    scene.add_drawable(solid);
    solid->position.x = 3 * cos(2*M_PI*glfwGetTime() * 0.2);
    solid->position.z = -3 * sin(2*M_PI*glfwGetTime() * 0.2);
    scene.draw_scene();
    framebuffer.end_render();

    //render the post-shaded result
    scene.drawables.clear();
    scene.add_drawable(sprite);
    scene.draw_scene();
  }
}
