#include <graphics/scene.h>
#include <graphics/object3d.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace graphics;

int main(int argc, char* args[]) {
  shared_ptr<Canvas> canvas = make_shared<Canvas>();
  Scene3D scene(canvas);
  scene.camera->position = glm::vec3(0,5,10);
  ((PointLight*)scene.light.get())->position.y = 10;
  ((PointLight*)scene.light.get())->ambient = glm::vec3(0.8);

  auto box_geometry = Geometry::create_box();
  shared_ptr<Object3D> box = make_shared<Object3D>(box_geometry);
  box->scale = glm::vec3(3);
  box->position.x = 0;
  box->position.z = 3;
  box->material.color = glm::vec3(0.6, 0.1, 0.1);

  auto terrain_geometry = Geometry::create_terrain(50, 50);
  shared_ptr<Object3D> terrain = make_shared<Object3D>(terrain_geometry);
  terrain->scale = glm::vec3(2.5);
  terrain->position.x = -2;
  terrain->position.z = 2;
  terrain->material.color = glm::vec3(0.1, 0.6, 0.1);
  terrain->material.shininess = 64;

  auto flat_sphere_geometry = Geometry::create_flat_sphere(1, 20, 20);
  shared_ptr<Object3D> flat_sphere = make_shared<Object3D>(flat_sphere_geometry);
  flat_sphere->position.x = 1;
  flat_sphere->position.z = -2;
  flat_sphere->material.color = glm::vec3(0.3, 0.5, 0.5);

  auto smooth_sphere_geometry = Geometry::create_smooth_sphere(1, 20, 20);
  shared_ptr<Object3D> smooth_sphere = make_shared<Object3D>(smooth_sphere_geometry);
  smooth_sphere->position.x = 3;
  smooth_sphere->position.z = 2;
  smooth_sphere->material.color = glm::vec3(0.6, 0.6, 0.2);
  smooth_sphere->material.shininess = 1;

  scene.add_drawable(box);
  scene.add_drawable(flat_sphere);
  scene.add_drawable(smooth_sphere);
  scene.add_drawable(terrain);

  while(canvas->still_open()) {
    canvas->clear();
    scene.clear();
    ((PointLight*)scene.light.get())->position.x = 10 * sin(2.0*M_PI*glfwGetTime()/3);
    ((PointLight*)scene.light.get())->position.z = 10 * cos(2.0*M_PI*glfwGetTime()/3);
    box->rotation.y -= 0.3;
    flat_sphere->rotation.x += 0.3;
    terrain->rotation.y += 0.3;

    scene.draw_scene();
  }
}
