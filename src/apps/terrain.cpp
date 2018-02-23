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
  scene.camera->position = glm::vec3(5,10,5);
  ((PointLight*)scene.light.get())->position = glm::vec3(5,5,0);

  Geometry terrain_geometry = Geometry::create_terrain(50, 50);
  shared_ptr<Object3D> terrain = make_shared<Object3D>(terrain_geometry);
  terrain->scale = glm::vec3(5);

  scene.add_drawable(terrain);

  while(canvas->still_open()) {
    scene.clear();
    terrain->rotation.y += 0.3;

    canvas->poll_events();
    scene.draw_scene();
  }

//  std::vector<glm::vec3> positions = {
//    glm::vec3(0,0.5,0) ,
//    glm::vec3(-0.5,-0.5,0),
//    glm::vec3(0.5,-0.5,0)
//  };
//
//  std::vector<glm::vec3> positions1 = {
//    positions[0] - glm::vec3(0,0,1),
//    positions[1] - glm::vec3(0,0,1),
//    positions[2] - glm::vec3(0,0,1)
//  };
//
//  Geometry geometry({Facet(0,1,2)}, positions);
//  Geometry geometry1({Facet(0,1,2)}, positions1);
//  geometry += geometry1;
//  auto obj = make_shared<Object3D>(geometry);
//
//  obj->scale *= 4;
//  scene.add_drawable(obj);
//  while(canvas->still_open()) {
//    scene.clear();
//    obj->rotation.y += 0.3;
//
//    canvas->poll_events();
//    scene.draw_scene();
//  }
}
