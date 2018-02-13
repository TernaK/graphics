#include <graphics/scene.h>
#include <graphics/shape.h>
using namespace std;

int main(int argc, char* args[]) {
  shared_ptr<graphics::Canvas> canvas = make_shared<graphics::Canvas>();
  graphics::Scene3D scene(canvas);
  
  std::shared_ptr<graphics::Solid> solid = graphics::SolidShape::make_sphere(3, 10, 10);
  
  scene.add_drawable(solid);

  while(!glfwWindowShouldClose(canvas->window)) {
    solid->rotation.y += 1;
    solid->rotation.x += 1;
    scene.draw_scene();
  }
}
