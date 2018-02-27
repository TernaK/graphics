#include <graphics/scene.h>
#include <graphics/object3d.h>
#include <graphics/scene_renderer.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace graphics;

int main(int argc, char* args[]) {
  shared_ptr<Canvas> canvas = make_shared<Canvas>();
  Light light;
  Camera camera;
  camera.aspect_ratio = canvas->get_aspect_ratio();
  camera.position = glm::vec3(0,5,10);
  light.position.y = 10;
  light.ambient = glm::vec3(0.8);
//
//  auto box_geometry = make_shared<PrimitiveGeometry>(Primitive(PrimitiveType::box));
//  shared_ptr<Object3D> box = make_shared<Object3D>(box_geometry);
//  box->scale = glm::vec3(2);
//  box->position.x = 0;
//  box->position.z = 3;
//  box->material.color = glm::vec3(0.6, 0.1, 0.1);

  auto terrain_geometry = Geometry::create_terrain(50, 50);
  shared_ptr<Object3D> terrain = make_shared<Object3D>(terrain_geometry);
  terrain->scale = glm::vec3(2.5);
  terrain->position.x = -2;
  terrain->position.z = 2;
  terrain->material.color = glm::vec3(0.1, 0.6, 0.1);
  terrain->material.shininess = 64;

  auto flat_sphere_geometry = make_shared<PrimitiveGeometry>(PrimitiveType::flat_sphere);
  shared_ptr<Object3D> flat_sphere = make_shared<Object3D>(flat_sphere_geometry);
  flat_sphere->position.x = 1;
  flat_sphere->position.z = 1;
  flat_sphere->material.color = glm::vec3(0.3, 0.5, 0.5);

  auto smooth_sphere_geometry = make_shared<PrimitiveGeometry>(PrimitiveType::smooth_sphere);
  shared_ptr<Object3D> smooth_sphere = make_shared<Object3D>(smooth_sphere_geometry);
  smooth_sphere->position.x = 3;
  smooth_sphere->position.z = 2;
  smooth_sphere->material.color = glm::vec3(0.6, 0.6, 0.2);
  smooth_sphere->material.shininess = 1;

//  box->add_child(flat_sphere);
  terrain->geometry->wire_frame = true;

  auto shader = Shader::make_object3d_point_shader();

  while(canvas->still_open()) {
    canvas->clear();

    shader->use();

    light.set_uniforms(shader);
    camera.set_uniforms(shader);

//    box->rotation.y -= 0.3;
    flat_sphere->rotation.x += 0.3;
    terrain->rotation.y += 0.3;

//    box->draw(shader);
    flat_sphere->draw(shader);
    terrain->draw(shader);
    smooth_sphere->draw(shader);

    canvas->poll_events();
    canvas->swap_buffers();
  }
}
