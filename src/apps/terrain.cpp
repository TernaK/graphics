#include <graphics/scene.h>
#include <graphics/object3d.h>
#include <graphics/buffer_object.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace graphics;

int main(int argc, char* args[]) {
  shared_ptr<Canvas> canvas = make_shared<Canvas>();
  Scene3D scene(canvas);
  scene.camera->position = glm::vec3(0,6,6);
  ((PointLight*)scene.light.get())->position = glm::vec3(10,10,10);

  auto terrain_geometry = Geometry::create_terrain(50, 50);
  shared_ptr<Object3D> terrain = make_shared<Object3D>(*terrain_geometry);
  terrain->scale = glm::vec3(2.5);
  terrain->position.x = -2;
  
  auto box_geometry = Geometry::create_box();
  shared_ptr<Object3D> box = make_shared<Object3D>(*box_geometry);
  box->scale = glm::vec3(2);
  box->position.x = 3;

  scene.add_drawable(terrain);
  scene.add_drawable(box);

  while(canvas->still_open()) {
    scene.clear();
    terrain->rotation.y += 0.3;
    box->rotation.y -= 0.3;

    canvas->poll_events();
    scene.draw_scene();
  }
}
