#include <graphics/scene.h>
#include <graphics/shape.h>
using namespace std;

int main(int argc, char* args[]) {
  shared_ptr<graphics::Canvas> canvas = make_shared<graphics::Canvas>();
  graphics::Scene3D scene(canvas);
  
  std::shared_ptr<graphics::Solid> earth = graphics::SolidShape::make_sphere(2, 10, 10);
  earth->material.color = glm::vec3(0.1, 0.1, 0.8);
  std::shared_ptr<graphics::Solid> moon = graphics::SolidShape::make_sphere(0.3, 7, 7);
  moon->material.color = glm::vec3(0.2, 0.8, 0.2);
  std::shared_ptr<graphics::Solid> mars = graphics::SolidShape::make_sphere(0.3, 7, 7);
  mars->material.color = glm::vec3(0.8, 0.1, 0.2);

  scene.add_drawable(earth);
  scene.add_drawable(moon);
  scene.add_drawable(mars);
  scene.clear_color = glm::vec4(1.0);

  while(canvas->still_open()) {
    earth->rotation.y += 1;
    earth->rotation.x += 1;
    moon->rotation.x += 1;
    moon->position.x = 3 * cos(2*M_PI*glfwGetTime() * 0.6);
    moon->position.z = -3 * sin(2*M_PI*glfwGetTime() * 0.6);
    mars->position.y = 3 * cos(2*M_PI*glfwGetTime() * 0.6);
    mars->position.z = 3 * sin(2*M_PI*glfwGetTime() * 0.6);
    mars->position.x = 1 * sin(2*M_PI*glfwGetTime() * 0.6);
    earth->material.color.b = abs(sin(2*M_PI*glfwGetTime() * 0.1));
    earth->material.color.r = 1 - earth->material.color.b;
    moon->material.color.g = abs(sin(2*M_PI*glfwGetTime() * 0.2));
    moon->material.color.b = 1 - moon->material.color.g;
    scene.draw_scene();
  }
}
