#include <graphics/scene.h>
#include <graphics/object3d.h>
#include <graphics/scene_renderer.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace graphics;

int main(int argc, char* args[]) {
  shared_ptr<Canvas> canvas = make_shared<Canvas>();
  shared_ptr<Scene> scene = make_shared<Scene>(canvas);

  scene->camera->position = glm::vec3(0,5,10);

  auto box_geometry = make_shared<PrimitiveGeometry>(PrimitiveType::box);
  shared_ptr<Object3D> box = make_shared<Object3D>(box_geometry);
  box->name = "box";
  box->position.x = 0;
  box->position.z = 3;
  box->material.color = glm::vec3(0.6, 0.1, 0.1);
  box->material.shininess = 32;

  auto terrain_geometry = Geometry::create_terrain(50, 50);
  shared_ptr<Object3D> terrain = make_shared<Object3D>(terrain_geometry);
  terrain->name = "terrain";
  terrain->scale = glm::vec3(4);
  terrain->position.x = -2;
  terrain->position.z = 2;
  terrain->material.color = glm::vec3(0.1, 0.6, 0.1);
  terrain->material.shininess = 64;

  auto flat_sphere_geometry = make_shared<PrimitiveGeometry>(PrimitiveType::flat_sphere);
  shared_ptr<Object3D> flat_sphere = make_shared<Object3D>(flat_sphere_geometry);
  flat_sphere->name = "flat_sphere";
  flat_sphere->position = glm::vec3(0, 2, 1);
  flat_sphere->material.color = glm::vec3(0.3, 0.5, 0.5);

  auto plane_geometry = make_shared<PrimitiveGeometry>(PrimitiveType::plane);
  shared_ptr<Object3D> plane = make_shared<Object3D>(plane_geometry);
  plane->scale = glm::vec3(10);
  plane->position = glm::vec3(0,-2,0);
  plane->material.color = glm::vec4(0.3);

  auto smooth_sphere_geometry = make_shared<PrimitiveGeometry>(PrimitiveType::smooth_sphere);
  shared_ptr<Object3D> smooth_sphere = make_shared<Object3D>(smooth_sphere_geometry);
  smooth_sphere->name = "smooth_sphere";
  smooth_sphere->position = glm::vec3(3,0,0);
  smooth_sphere->material.color = glm::vec3(0.1, 0.2, 0.6);
  smooth_sphere->material.shininess = 256;
  smooth_sphere->material.strength.y = 0.9;
  smooth_sphere->material.shininess = 2;

  box->add_child(flat_sphere);
  terrain->geometry->wire_frame = true;

  auto light_node = make_shared<Object3D>();
  light_node->requires_camera = false;
  light_node->requires_shader = false;
  auto light = make_shared<Light>(LightType::directional);
  light_node->light = light;

  scene->add_node(light_node);
  scene->add_node(box);
  scene->add_node(terrain);
  scene->add_node(smooth_sphere);
  scene->add_node(plane);

  SceneRenderer renderer;

  while(canvas->still_open()) {
    scene->clear();

//    scene->root->rotation.y -= 0.3;
    flat_sphere->rotation.x += 0.3;
    terrain->rotation.y += 1;
    box->rotation.y += 2;

    renderer.render_scene(scene);

    canvas->poll_events();
    canvas->swap_buffers();
  }
}
