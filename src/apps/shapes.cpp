#include <graphics/scene.h>
#include <graphics/shape.h>
using namespace std;

int main(int argc, char* args[]) {
  shared_ptr<graphics::Canvas> canvas = make_shared<graphics::Canvas>();
  graphics::Scene3D scene(canvas);
  
  std::shared_ptr<graphics::Solid> earth = graphics::SolidShape::make_sphere(2, 10, 10);
  earth->material.color = glm::vec3(0.1, 0.1, 0.8);
  std::shared_ptr<graphics::Solid> moon = graphics::SolidShape::make_sphere(0.3, 7, 7);
  moon->material.color = glm::vec3(0.4, 0.4, 0.4);
  
  scene.add_drawable(earth);
  scene.add_drawable(moon);

  while(!glfwWindowShouldClose(canvas->window)) {
    earth->rotation.y += 1;
    earth->rotation.x += 1;
    moon->rotation.x += 1;
    moon->position.x = 3 * cos(2*M_PI*glfwGetTime() * 0.2);
    moon->position.z = -3 * sin(2*M_PI*glfwGetTime() * 0.2);
    scene.draw_scene();
  }
}
