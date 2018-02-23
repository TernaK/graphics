#include <graphics/scene.h>
#include <graphics/solid.h>
#include <graphics/vbo.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace graphics;

int main(int argc, char* args[]) {
  shared_ptr<Canvas> canvas = make_shared<Canvas>();
  Scene3D scene(canvas);
  scene.camera->position = glm::vec3(5,10,5);
  ((PointLight*)scene.light.get())->position = glm::vec3(5,5,0);

  Geometry terrain_geometry = Geometry::create_terrain(10, 10);
  shared_ptr<Solid> terrain = make_shared<Solid>(terrain_geometry);
  terrain->scale = glm::vec3(5);

  scene.add_drawable(terrain);

  while(canvas->still_open()) {
    scene.clear();
    terrain->rotation.y += 0.3;

    canvas->poll_events();
    scene.draw_scene();
  }
}
