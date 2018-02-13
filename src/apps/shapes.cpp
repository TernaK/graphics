#include <graphics/scene.h>
#include <graphics/shape.h>
using namespace std;

int main(int argc, char* args[]) {
  shared_ptr<graphics::Canvas> canvas = make_shared<graphics::Canvas>();
  graphics::Scene3D scene(canvas);
  
  std::shared_ptr<graphics::Solid> cube = graphics::SolidShape::make_cuboid(5.0, 2, 1);
  std::shared_ptr<graphics::Solid> sphere = graphics::SolidShape::make_sphere(1, 6, 6);
  
  scene.add_drawable(cube);

  while(!glfwWindowShouldClose(canvas->window)) {
    cube->rotation.y += 1;
    scene.draw_scene();
  }
}
