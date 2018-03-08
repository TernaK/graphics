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
  SceneRenderer renderer;

  scene->camera->position = glm::vec3(0,5,10);

  auto box_geometry = make_shared<Geometry>(ShapeType::box);
  shared_ptr<Object3D> box = make_shared<Object3D>(box_geometry);
  box->name = "box";
  box->position.x = -3;
  box->position.z = 3;
  box->material.color = glm::vec3(0.6, 0.1, 0.1);
  box->material.shininess = 32;

  auto terrain_params = PrimitiveParams(50,50);
  auto terrain_geometry = make_shared<Geometry>(ShapeType::terrain, terrain_params);
  shared_ptr<Object3D> terrain = make_shared<Object3D>(terrain_geometry);
  terrain->name = "terrain";
  terrain->scale = glm::vec3(4);
  terrain->position.x = -2;
  terrain->position.z = -2;
  terrain->material.color = glm::vec3(0.1, 0.6, 0.1);
  terrain->material.shininess = 64;

  auto flat_sphere_geometry = make_shared<Geometry>(ShapeType::flat_sphere);
  shared_ptr<Object3D> flat_sphere = make_shared<Object3D>(flat_sphere_geometry);
  flat_sphere->name = "flat_sphere";
  flat_sphere->position = glm::vec3(0, 2, 1);
  flat_sphere->material.color = glm::vec3(0.3, 0.5, 0.5);
  flat_sphere->geometry->wire_frame = true;

  auto plane_geometry = make_shared<Geometry>(ShapeType::plane);
  shared_ptr<Object3D> plane = make_shared<Object3D>(plane_geometry);
  plane->scale = glm::vec3(6);
  plane->position = glm::vec3(0,-2,0);
  plane->material.color = glm::vec4(0.9);

  auto smooth_sphere_geometry = make_shared<Geometry>(ShapeType::sphere);
  shared_ptr<Object3D> smooth_sphere = make_shared<Object3D>(smooth_sphere_geometry);
  smooth_sphere->name = "smooth_sphere";
  smooth_sphere->position = glm::vec3(3,0,0);
  smooth_sphere->material.color = glm::vec3(0.1, 0.2, 0.6);
  smooth_sphere->material.shininess = 256;
  smooth_sphere->material.strength.y = 0.9;
  smooth_sphere->material.shininess = 2;
  smooth_sphere->geometry->wire_frame = true;

  box->add_child(flat_sphere);
  terrain->geometry->wire_frame = true;
  auto light = make_shared<Light>(LightType::directional);
  light->type = LightType::directional;

  box->light = light;
  scene->add_node(box);
  scene->add_node(terrain);
  scene->add_node(smooth_sphere);
  scene->add_node(plane);


  while(canvas->still_open()) {
    scene->clear();

    scene->root->rotation.y -= 0.3;
    flat_sphere->rotation.x += 0.3;
    terrain->rotation.y += 1;
    box->rotation.y += 2;

    renderer.render_scene(scene);

    CanvasHelper::check_gl_errors();
    canvas->poll_events();
    canvas->swap_buffers();
  }
}
